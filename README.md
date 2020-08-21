# image_processing
Simple image processing on PGM files (adding effects). Some piece of codes were provided to students by their lecturers during the course provided at Wroclaw University of Science and Technology. Specification of the task is described [here](https://kcir.pwr.edu.pl/~mucha/PProg/PProg_lab_05/index.html) - the task was a little modified by our teacher.

## Important information 

### Prerequisites
After cloning this repository to your computer run the Makefile command in the main directory:
```
make
```
This command will create an _obrazy_ executable.

### Running the program
To run the program in your console you should run it this way:
```
./obrazy {[-i name_of_input_file] [-o name_of_output_file] [-p number] [-n] [-k] [-d] [-r] [-m] [-z number number] [-h]}
```
Where:
* _-i name_of_input_file_: give the name of the file you want to read
* _-o name_of_output_file_: give the name of the file, to which you want to write the output  
* _-p name_: give the value of threshold and do thresholding
* _-n_: use the function of negative
* _-k_: use the function of contouring
* _-d_: display the input image
* _-r_: use the function of blurring
* _-m_: add a simple mask to your image
* _-z number number_: use the function of level changing
* _-h_: display how the program should be run in the console (help)

### Documentation
All comments describing the program are written in Polish as it was required by our teachers.

### Authors
* **Emilia Szymańska** - *running the program with flags, applying effects etc.*
* **MSc Arkadiusz Mielczarek** & **Dr Robert Muszyński** - *initial code* (to help students start with developing the program)
