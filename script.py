import os
import pandas as pd
import sys

print("Running parameter space study...")

seed = 34534
allArrivalRates = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]
allMaxServiceTimes = [ 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10 ]

allPossiblePairs = [ ]
for arrivalRate in allArrivalRates:
    for maxServiceTime in allMaxServiceTimes:
        allPossiblePairs.append(str(arrivalRate) + " " + str(maxServiceTime))

exists = os.path.isfile("rawData.txt")
if not exists:
    print("rawData.txt file not found")
    sys.exit()

path = "rawData.txt"
all90thPercentiles = pd.read_csv(path, sep = " ", header = None)
all90thPercentiles = all90thPercentiles.iloc[:,-1]
bank90thPercentiles = [ ]
supermarket90thPercentiles = [ ]

i = 0
j = 0
k = 0
for value in all90thPercentiles:
    if i % 2 is 0: # is even
        bank90thPercentiles.append(allPossiblePairs[j] + " " + str(value))
        j += 1
    if i % 2 is 1: # is odd
        supermarket90thPercentiles.append(allPossiblePairs[k] + " " + str(value))
        k += 1
    i += 1

bankFile = open("bankData.txt","w") 
bankFile.write("CustomerArrivalRate MaxCustomerServiceTime AverageWaitPerPerson\n")
for datapoint in bank90thPercentiles:
    bankFile.write(str(datapoint) + "\n")

supermarketFile = open("supermarketData.txt","w") 
supermarketFile.write("CustomerArrivalRate MaxCustomerServiceTime AverageWaitPerPerson\n")
for datapoint in supermarket90thPercentiles:
    supermarketFile.write(str(datapoint) + "\n")


# output format
#
# BANK
# arrivalRate1 serviceTimeMax1 datapoint1
# arrivalRate2 serviceTimeMax2 datapoint2
# arrivalRate3 serviceTimeMax3 datapoint3

#
# SUPERMARKET
# arrivalRate1 serviceTimeMax1 datapoint1
# arrivalRate2 serviceTimeMax2 datapoint2
# arrivalRate3 serviceTimeMax3 datapoint3
