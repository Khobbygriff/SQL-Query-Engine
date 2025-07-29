#include "database/database.h"
#include <iostream>

int main() {
    Database db;

    db.createTable("students", {"id", "name", "gpa"});
    db.createTable("courses", {"course_id", "student_id", "course_name"});
    db.insert("students", {{"id", 1}, {"name", "Alice"}, {"gpa", 3.8}});
    db.insert("students", {{"id", 2}, {"name", "Bob"}, {"gpa", 3.5}});
    db.insert("students", {{"id", 3}, {"name", "Charlie"}, {"gpa", 3.2}});
    db.insert("courses", {{"course_id", 101}, {"student_id", 1}, {"course_name", "Math"}});
    db.insert("courses", {{"course_id", 102}, {"student_id", 2}, {"course_name", "Physics"}});
    db.insert("courses", {{"course_id", 103}, {"student_id", 1}, {"course_name", "Chemistry"}});

    db.createIndex("students", "id");

    std::cout << "Query 1: SELECT name, gpa FROM students WHERE id = 2" << std::endl;
    db.execute("SELECT name, gpa FROM students WHERE id = 2");

    std::cout << "\nQuery 2: SELECT name FROM students WHERE gpa > 3.4" << std::endl;
    db.execute("SELECT name FROM students WHERE gpa > 3.4");

    std::cout << "\nQuery 3: JOIN students and courses on students.id = courses.student_id" << std::endl;
    db.execute("SELECT students.name, courses.course_name FROM students JOIN courses ON students.id = courses.student_id");

    std::cout << "\nQuery 4: Aggregation - SELECT AVG(gpa) FROM students" << std::endl;
    db.execute("SELECT AVG(gpa) FROM students");

    std::cout << "\nTransaction Demo:" << std::endl;
    db.beginTransaction();
    db.insert("students", {{"id", 4}, {"name", "Diana"}, {"gpa", 3.9}});
    std::cout << "Inserted Diana, but not committed yet. Querying students:" << std::endl;
    db.execute("SELECT name FROM students");
    db.rollback();
    std::cout << "Rolled back. Querying students again:" << std::endl;
    db.execute("SELECT name FROM students");

    return 0;
}