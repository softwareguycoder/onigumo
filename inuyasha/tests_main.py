#!/usr/bin/python3
from directory_utilities_tests import DirectoryUtilitiesTests


def runDirectoryUtilitiesTests():
    DirectoryUtilitiesTests.TestRemoveDirectory()
    
    DirectoryUtilitiesTests.TestCreateDirectory()
    
    DirectoryUtilitiesTests.TestSetCurrentDirectory()
    
    DirectoryUtilitiesTests.TestExists()


def main():
    runDirectoryUtilitiesTests()


if __name__ == "__main__":
    main()
