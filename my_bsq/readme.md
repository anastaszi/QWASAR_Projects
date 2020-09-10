# The Biggest square on a map

The objective of this project is to find the largest possible square on a board while avoiding obstacles.\

- [x] Memory efficient : bit array is used to store map;
- [x] Fast: dynamic programming approach to solve the task;
- [x] Clean architecture: OOP in C implemented;

### How to use

1. Compile the program: run `make`
2. Create new map/board: run `./maps_generator [width] [height] [density] > board.txt`
maps_generator will generate a X by Y map with obstacles of D density and save it to file 'board.txt'
3. Check the map: run `cat board.txt`
4. Run my_bsq: `./my_bsq board.txt`


![Steps](https://github.com/anastaszi/QWASAR_Projects/blob/master/my_bsq/img/my_bsq_example.png)
