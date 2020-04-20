#include "headers/rgb.h"
#include "headers/image.h"
#include "headers/colour_palette.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <cmath>
#include <ctime>
#include <mpi.h>
#include <omp.h>
using namespace std;

// Colour palette parameters
string colour_palette_filename;
double colour_palette_min, colour_palette_max;
bool cubic_spline;
string RGBspace_filename;
unsigned RGBspace_partition;
string RGBpalette_filename;

// Image parameters
unsigned img_width, img_height;
string img_filename;
double real_min, real_max, imaginary_min, imaginary_max;
unsigned max_iteration;

bool init(const char *filename);

#define TAG_RESULT 0
#define TAG_ASK_FOR_JOB 1
#define TAG_JOB_DATA 2
#define TAG_STOP 3

double master(unsigned world_size);
void worker(void);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " init_params.file" << endl;
        return 1;
    }

    clock_t global_begin, global_end;
    double global_secs, fractal_secs;

    global_begin = clock();

    if (!init(argv[1])) return 1;

    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) fractal_secs = master(world_size);
    else worker();

    global_end = clock();

    global_secs = (global_end - global_begin) / (double)CLOCKS_PER_SEC;

    if (world_rank == 0)
        cout << global_secs << "\t" << fractal_secs << endl;

    MPI_Finalize();
    return 0;
}

bool init(const char *filename)
{
    ifstream ifs(filename);
    string line;
    try {
        if (ifs.fail()) throw("Can't open input file");

        getline(ifs, colour_palette_filename);

        getline(ifs, line);
        if (line.compare("true") == 0) {
            getline(ifs, line); getline(ifs, line);
            colour_palette_min = 0;
            colour_palette_max = M_PI / 2.0;
        }
        else if (line.compare("false") == 0) {
            getline(ifs, line); colour_palette_min = stod(line);
            getline(ifs, line); colour_palette_max = stod(line);
        }
        else throw("WoW");

        getline(ifs, line);
        if (line.compare("true") == 0) cubic_spline = true;
        else if (line.compare("false") == 0) cubic_spline = false;
        else throw("WoW");

        getline(ifs, RGBspace_filename);
        getline(ifs, line); RGBspace_partition = stoi(line);
        getline(ifs, RGBpalette_filename);

        getline(ifs, line); img_width = stoi(line);
        getline(ifs, line); img_height = stoi(line);
        getline(ifs, img_filename);

        getline(ifs, line); real_min = stod(line);
        getline(ifs, line); real_max = stod(line);
        getline(ifs, line); imaginary_min = stod(line);
        getline(ifs, line); imaginary_max = stod(line);

        getline(ifs, line); max_iteration = stoi(line);

        img_filename = img_filename + "fractal_" + to_string(img_width) + 'x' + to_string(img_height) + '_' + to_string(max_iteration) + ".ppm";

        ifs.close();
        return true;
    }
    catch (const char *err) {
        cout << err << "\n";
        ifs.close();
        return false;
    }
}

double master(unsigned world_size)
{
    clock_t fractal_begin, fractal_end;

    Image fractal(img_width, img_height);

    unsigned worker_size = world_size - 1;
    cout << "Master 0 of " << world_size << " processes, " << worker_size << " workers" << endl;

    unsigned workers_working = worker_size, worker_rank, job = 0;
    unsigned worker_job[world_size];
    MPI_Status stat, stat2;

    // Creamos nuestro datatype para RGB
    MPI_Datatype RGB_type;
    int lengths[] = {1, 1, 1};
    const MPI_Aint displacements[] = {0, sizeof(double), sizeof(double) + sizeof(double)};
    MPI_Datatype types[] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Type_create_struct(3, lengths, displacements, types, &RGB_type);
    MPI_Type_commit(&RGB_type);

    RGB buffer[img_width];

    MPI_Barrier(MPI_COMM_WORLD);
    fractal_begin = clock();

    while (workers_working) {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        worker_rank = stat.MPI_SOURCE;
        if (stat.MPI_TAG == TAG_ASK_FOR_JOB) {
            MPI_Recv(NULL, 0, MPI_BYTE, worker_rank, TAG_ASK_FOR_JOB, MPI_COMM_WORLD, &stat2);

            if (job < img_height) {
                MPI_Send(&job, 1, MPI_UNSIGNED, worker_rank, TAG_JOB_DATA, MPI_COMM_WORLD);
                worker_job[worker_rank] = job;
                job++;
            }
            else {
                MPI_Send(NULL, 0, MPI_BYTE, worker_rank, TAG_STOP, MPI_COMM_WORLD);
                workers_working--;
            }
        }
        else if (stat.MPI_TAG == TAG_RESULT){
            MPI_Recv(buffer, img_width, RGB_type, worker_rank, TAG_RESULT, MPI_COMM_WORLD, &stat2);
            memcpy(fractal.pixels + worker_job[worker_rank] * img_width, buffer, sizeof(RGB) * img_width);
        }
        else {
            cout << "[-] ERROR" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }


    fractal_end = clock();

    savePPM(fractal, img_filename.c_str());
    return (fractal_end - fractal_begin) / (double)CLOCKS_PER_SEC;
}
void worker(void)
{
    // Definimos el espacio RGB
    colour_palette RGBspace(colour_palette_min, colour_palette_max);
    RGBspace.set_points(colour_palette_filename.c_str(), cubic_spline);
    // RGBspace.plot_RGBspace(RGBspace_filename.c_str(), RGBspace_partition);
    // RGBspace.plot_palette(RGBpalette_filename.c_str());

    // Creamos nuestro datatype para RGB
    MPI_Datatype RGB_type;
    int lengths[] = {1, 1, 1};
    const MPI_Aint displacements[] = {0, sizeof(double), sizeof(double) + sizeof(double)};
    MPI_Datatype types[] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Type_create_struct(3, lengths, displacements, types, &RGB_type);
    MPI_Type_commit(&RGB_type);

    RGB buffer[img_width];
    unsigned y, x, iteration;
    double range_width = (real_max - real_min) / (double)(img_width - 1);
    double range_height = (imaginary_max - imaginary_min) / (double)(img_height - 1);
    double zr, zi, cr, ci, aux;

    MPI_Status stat, stat2;

    MPI_Barrier(MPI_COMM_WORLD);

    bool stop = false;
    do {
        MPI_Send(NULL, 0, MPI_BYTE, 0, TAG_ASK_FOR_JOB, MPI_COMM_WORLD);

        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        if (stat.MPI_TAG == TAG_JOB_DATA) {
            MPI_Recv(&y, 1, MPI_UNSIGNED, 0, TAG_JOB_DATA, MPI_COMM_WORLD, &stat2);

            #pragma omp parallel for ordered schedule(dynamic)
            for (x = 0; x < img_width; ++x) {
                zr = zi = 0.0;
                cr = x * range_width + real_min;
                ci = y * range_height + imaginary_min;

                iteration = 0;
                while (iteration < max_iteration && (zr * zr + zi * zi) < 4.0) {
                    aux = zr * zr - zi * zi + cr;
                    zi = fabs(2 * zr * zi + ci);
                    zr = fabs(aux);
                    iteration++;
                }

                if (iteration == max_iteration) {

                    #pragma omp ordered
                    buffer[x] = kWhite;

                }
                else {

                    #pragma omp ordered
                    buffer[x] = RGBspace((double)iteration);
                    
                }
            }

            MPI_Send(buffer, img_width, RGB_type, 0, TAG_RESULT, MPI_COMM_WORLD);
        }
        else if (stat.MPI_TAG == TAG_STOP) {
            MPI_Recv(NULL, 0, MPI_BYTE, 0, TAG_STOP, MPI_COMM_WORLD, &stat2);
            stop = true;
        }
    } while (!stop);
}
