# Find the names of courses in the Computer science department which have 3 credits. Hint: Use the course table.
SELECT university_db.course.title FROM university_db.course WHERE credits = 3 AND dept_name = "Comp. Sci.";



# For the student with ID 76543, show the course id and the title of all courses registered for by the student. Hint: Use the course and takes tables.
SELECT university_db.course.course_id,university_db.course.title 
	FROM university_db.takes, university_db.course 
    WHERE ID = 76543 AND course.course_id=takes.course_id;

# Find the departments that have more than two instructors (not including two). Hint: Use the instructor table.
SELECT dept_name FROM university_db.instructor GROUP BY dept_name HAVING COUNT(name) > 2;

SELECT AVG(salary) as avgsal FROM university_db.instructor WHERE dept_name = "Comp. Sci.";

#SELECT * from university_db.instructor;
#SELECT name,dept_name from university_db.instructor;
#SELECT COUNT(DISTINCT name) AS N from university_db.instructor 
#SELECT COUNT(DISTINCT name) AS num FROM university_db.instructor WHERE num>1
#SELECT university_db.instructor.dept_name FROM university_db.instructor 
#SELECT DISTINCT university_db.instructor.dept_name,university_db.instructor 
#	FROM university_db.instructor 
 #   WHERE (SELECT COUNT(DISTINCT name) FROM university_db.instructor WHERE instructor.name = ) > 2
	#	AND 
#SELECT * FROM university_db.department; #useless
#SELECT * FROM university_db.instructor,university_db.teaches WHERE instructor.ID=teaches.ID;
#SELECT DISTINCT name,dept_name FROM university_db.instructor, university_db.teaches;
#SELECT COUNT(DISTINCT name) FROM university_db.instructor WHERE dept_name=instructor.dept_name AND COUNT(DISTINCT name) > 2;
#SELECT DISTINCT dept_name from university_db.instructor WHERE (SELECT COUNT(DISTINCT name) FROM university_db.instructor WHERE dept_name=instructor.dept_name) > 2;

