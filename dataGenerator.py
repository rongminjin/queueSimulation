import os
import sys

os.system("rm -f rawData.txt") # remove old data

seed = 34534
allArrivalRates = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]
allMaxServiceTimes = [ 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10 ]

for arrivalRate in allArrivalRates:
    for maxServiceTime in allMaxServiceTimes:
        os.system("./simulator " + str(arrivalRate) + " " + str(maxServiceTime) + " " + str(seed) + " >> rawData.txt")
