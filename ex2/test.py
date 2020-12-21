from eventManager import *

if __name__ == '__main__':
    fileCorrect("tests/input","tests/out1")
    num1 = correctAgeAvg("tests/input", 3)
    print(float(num1))
    printYoungestStudents("tests/input","tests/out2", 2)
