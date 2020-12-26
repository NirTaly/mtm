#### IMPORTS ####
import event_manager as EM

# parse file into list
def linesFromFile(orig_file_path: str) -> list:
	read_file = open(orig_file_path, 'r') 

	d = {}
	for line in read_file: 
		line = " ".join(line.split())
		line = line.replace(' ,', ',')
		line = line.replace(', ',  ',')
		line = line.replace(',',  ', ')
		line = line.rsplit(', ')
		
		if isValidLine(line) is True:
			d[line[0]] = line
		
	line_list = []
	for key in sorted(d):
		line_list.append(d[key])

	read_file.close()

	return line_list

# check if line is valid
def isValidLine(line: list) -> bool:
	# id = str(line[0])
	id = str(int(line[0]))
	name = str(line[1]).replace(' ','')
	age = int(line[2])
	birth_year = int(line[3])
	semester = int(line[4])
	
	if (len(id) != 8) or (not name.isalpha()) or (age + birth_year != 2020) \
                      or (not (16 <= age <= 120)) or (semester < 1):
		return False

	return True

#### PART 1 ####
# Filters a file of students' subscription to specific event:
#   orig_file_path: The path to the unfiltered subscription file
#   filtered_file_path: The path to the new filtered file
def fileCorrect(orig_file_path: str, filtered_file_path: str):
	write_file = open(filtered_file_path, 'w')

	list_lines = linesFromFile(orig_file_path)

	for line in list_lines:
		write_file.write(line[0] + ', ' + line[1] + ', ' + line[2] + ', ' + line[3] + ', ' + line[4]+'\n')
    
	write_file.close()

    
# Writes the names of the K youngest students which subscribed 
# to the event correctly.
#   in_file_path: The path to the unfiltered subscription file
#   out_file_path: file path of the output file
def printYoungestStudents(in_file_path: str, out_file_path: str, k: int) -> int:
	if k < 1:
		return -1
	
	lines_list = linesFromFile(in_file_path)
	sorted_lines = sorted(lines_list, key=lambda elem: (int(elem[2]),int(elem[0])))
	
	write_file = open(out_file_path,'w')
	for i , line in enumerate(sorted_lines):
		if i == k:
			break
		write_file.write(line[1] + '\n')

	write_file.close()
	return i if k < len(sorted_lines) else len(sorted_lines)

# Calculates the avg age for a given semester
#   in_file_path: The path to the unfiltered subscription file
#   retuns the avg, else error codes defined.
def correctAgeAvg(in_file_path: str, semester: int) -> float:
	if(semester < 1):
		return -1
	
	list_lines = linesFromFile(in_file_path)

	count = 0
	total_age = 0
	for line in list_lines:
		if(int(line[4]) == semester):
			total_age += int(line[2])
			count += 1
	
	return  (total_age/count) if count != 0 else 0
    

#### PART 2 ####
# Use SWIG :)
# print the events in the list "events" using the functions from hw1
#   events: list of dictionaries
#   file_path: file path of the output file
def printEventsList(events :list,file_path :str): #em, event_names: list, event_id_list: list, day: int, month: int, year: int):
    # sorted_events = sorted(events, key=lambda event: event[2])
    dates = []
    for event in events:
        dates.append(event['date'])
    
    min_date = dates[0]
    for date in dates:
        if EM.dateCompare(date,min_date) < 0:
            min_date = date

    em = EM.createEventManager(min_date)

    for event in events:
        EM.emAddEventByDate(em,event['name'],event['date'],event['id'])
    
    EM.emPrintAllEvents(em,file_path)
    return em

def testPrintEventsList(file_path :str):
    events_lists=[{"name":"New Year's Eve","id":1,"date": EM.dateCreate(30, 12, 2020)},\
                    {"name" : "annual Rock & Metal party","id":2,"date":  EM.dateCreate(21, 4, 2021)}, \
                                 {"name" : "Improv","id":3,"date": EM.dateCreate(13, 3, 2021)}, \
                                     {"name" : "Student Festival","id":4,"date": EM.dateCreate(13, 5, 2021)},    ]
    em = printEventsList(events_lists,file_path)
    for event in events_lists:
        EM.dateDestroy(event["date"])
    EM.destroyEventManager(em)

#### Main #### 
# feel free to add more tests and change that section. 
# sys.argv - list of the arguments passed to the python script
if __name__ == "__main__":
    import sys
    if len(sys.argv)>1:
        testPrintEventsList(sys.argv[1])