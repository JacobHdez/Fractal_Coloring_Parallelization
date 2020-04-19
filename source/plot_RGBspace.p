# Set the output to a png file
set terminal png size 500,500

# The file we'll write to
set output "../results/RGB_space.png"

# The graphic title
set title "RGB space"

#plot the graphic
plot [0:1024] [0:1] "../results/RGB_points.txt" using 1:2 w l lt 7 title "red", \
                    "../results/RGB_points.txt" using 1:3 w l lt 10 title "green", \
                    "../results/RGB_points.txt" using 1:4 w l lt 14 title "blue"
