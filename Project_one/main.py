"""Alex Selyutin
The following code analyzes a csv file full of student grades to select students based on four factors (based on grades earned)
Then, the program outputs the selected applicants (ones who passed all four tests)"""

import csv

def analyze_applicant1 (applicant_scores):
        """ Given 6 scores of an applicant, return True if:
        the applicants average on all six grades is above 85
        @param applicant_scores: a list of 6 scores (ints)
        @return a boolean -> True if average >85; false otherwise
        """

        sum = 0
        #loop through 6 scores and add each score to sum
        for i in applicant_scores: 
                sum+=int (i)
        #compute average for a given student
        average = sum/ (len(applicant_scores)) 
        #return if a condition is satisfied
        if average > 85: 
                return True 
        else: return False
#______________________________________ 

def analyze_applicant2(applicant_scores):
        """ Given 6 scores of an applicant, return True if:
        the applicant does not have a grade below 65
        @param applicant_scores: a list of 6 scores (ints)
        @return a boolean -> True if no grade <65 is found; false otherwise
        """

        #loop through the list
        for i in applicant_scores:
                score = int (i) #every score is initially a string -> needs to be cast as an int for computation
                if score < 65: # noone with a grade less than 65 will be accepted
                        return False
        
        return True # if all grades were above or at 65
#______________________________________ 

def analyze_applicant3(applicant_scores):
        """ Given 6 scores of an applicant, return True if:
        the applicant has at least 4 grades above 85 (non-cs counts as a grade)
        @param applicant_scores: a list of 6 scores (ints)
        @return a boolean -> True if # of grades above 85 is four or more; false otherwise
        """

        count_grades_above_85 = 0
        for i in applicant_scores:
                if int (i) > 85 :
                        count_grades_above_85 +=1 #increment until there are 4 (that's when the condition is met)
                        if count_grades_above_85 == 4:
                                return True;
        return False; # would have returned True once the 4th grade is above 85
#______________________________________ 
""" order of fields in a file: 
#IntroToCS,DataStructures,Algorithms,ComputerOrganization,OS,       NonCS
"""

def analyze_applicant4(applicant_scores):
        """ Given 6 scores of an applicant, return True if:
        the average for 5 CS courses (first five fields in the list) is above 85 (86 min)
        @param applicant_scores: a list of 6 scores (ints)
        @return a boolean -> True if CS GPA average at least 85; false otherwise
        """

        score_sum=0
        for x in range (5): #do not account for the sixth Non-CS GPA
              score_sum += int(applicant_scores[x])# string cast as an int
        
        #compute CS average
        average = score_sum/5
        if average >85 : #in CS courses only; dismiss the last Non-cs grades
                ave = average
                return True

        else: return False
#______________________________________ 


#MAIN
filename = "applicants.csv" #applicant records

fields = []
rows = []

#open file filename for reading ('r')
with open (filename, 'r') as csvfile: #open in read mode; file object named csvfile
        csvreader = csv.reader (csvfile) #creating a csv reader object

        fields = next (csvreader) #the names of the classes (first row of the csv file)
        #extracting each data row one by one
        for row in csvreader :
                rows.append (row)
                

#open file named 'results.csv' for writing ('w')
with open ('results.csv', 'w',newline = "") as csvfile:
        csvwriter = csv.writer (csvfile)
        #if the header is needed, it can be printed to the file with the next line
        csvwriter.writerow (fields)

        for i in rows: # iterate through every row and run all four test for every row of student scores
                #the following if statement will generate the output matching the names of the students in the source file (same lines)
                if analyze_applicant1(i) and analyze_applicant2(i) and analyze_applicant3(i) and analyze_applicant4(i):
                        csvwriter.writerow (['ACCEPT'] )
                else: csvwriter.writerow (["REJECT"])

#end main
#edited last on aug 26, 9 PM
