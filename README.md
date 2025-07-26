# SQL-Query-Engine
A C++ SQL query processor project

Here’s a clean, clear onboarding message you can send to your teammates—whether it’s through WhatsApp, Discord, or posted in the repo’s README.md:

---

### 📢 **Team Instructions for Working on the SQL Query Engine Project**

Hey team! Welcome aboard 🎉 Here's everything you need to know to start working effectively on your module.

---

#### 🧭 **Step 1: Clone the Repository**

Open Git Bash or terminal and run:

```bash
git clone https://github.com/Khobbygriff/SQL-Query-Engine.git
cd SQL-Query-Engine
```

---

#### 🌿 **Step 2: Switch to Your Assigned Branch**

Each member works on a specific module branch. Use the command below with your assigned branch name:

```bash
git checkout parser-module         # Griffith
git checkout planner-module        # Mark
git checkout aggregation           # Stella
git checkout join-algorithms       # Festus
git checkout indexing              # Eliana
git checkout transactions          # Nathaniel
git checkout integration-testing   # [Final integration lead]
```

---

#### 🛠️ **Step 3: Work Inside Your Module Folder**

Each branch has a corresponding folder (e.g., `parser/`, `planner/`, etc.).  
Place your `.cpp` and `.h` files inside that folder and commit regularly.

```bash
git add .
git commit -m "Added query planning function"
git push origin planner-module
```

---

#### 📥 **Step 4: Sync and Review**

Before starting your day:

```bash
git pull origin planner-module
```

Once your module is ready, create a **Pull Request** from your branch into `main` so we can review and merge.

---

#### 📚 **Bonus Tips**

- Comment your code clearly.
- Use the `README.md` inside your folder (if needed) to explain your logic.
- Don't push to `main` directly — always use pull requests.

---

*Individual Responsibilities*

integration-testing: All

parser-module: Griffith

planner-module: Mark

aggregation: Stella

join-algorithms: Festus

indexing: Eliana

transactions: Nathaniel

      NOTES ON SQL

---

 SQL
SQL stands for Structured Query Language.  
It is a standard language used to manage and query data in relational databases.  
With SQL, you can create tables, insert data, update data, delete data, and retrieve data using queries.

---

 SQL Query
An SQL query is a command written in SQL to perform operations on a database.  
Examples:
- `SELECT name FROM students WHERE gpa > 3.4;` (retrieves data)
- `INSERT INTO students (id, name, gpa) VALUES (4, 'Diana', 3.9);` (adds data)

Queries can filter, join, aggregate, and manipulate data.

---

 SQL Query Engine
An SQL query engine is a software component that:
- Parses SQL queries (break down and analyze a piece of text (like an SQL query) into its meaningful components, so a computer program can understand and work with it.)
- Plans how to execute them (decides the best way to get results)
- Executes the plan (retrieves or modifies data as requested)

It translates SQL queries into actions on the underlying data structures (tables, rows, etc.), returning results to the user.

---

 What does it do?
A query engine:
- Lets you interact with data using SQL queries.
- Finds and returns data based on your requests.
- Supports features like filtering (`WHERE`), joining tables (`JOIN`), aggregating data (`AVG`, `SUM`, `COUNT`), and transactions (`BEGIN`, `COMMIT`, `ROLLBACK`).
- Optimizes queries using indexes for faster lookups.

In this code, the SQL query engine works in-memory (no external database), making it fast and easy to experiment with SQL features in C++.

This code implements a mini in-memory SQL query engine in C++.  
It supports basic SQL features: `SELECT`, `WHERE`, `JOIN`, `AVG/SUM/COUNT` aggregation, and simple transaction management.

---

 Main Components

 1. Data Structures
- Value: A variant type (`int`, `double`, `string`) for cell values.
- Row: An unordered map from column name to `Value`.
- Table: A vector of `Row`s.

 2. AST (Abstract Syntax Tree)
- NodeType: Enum for SQL clause types (`SELECT`, `FROM`, `WHERE`, `JOIN`, etc.).
- ASTNode: Represents a node in the parsed SQL query tree.

 3. SQLParser
- Purpose: Tokenizes and parses SQL strings into an AST.
- Features: Handles column lists, aggregation functions, JOINs, and WHERE clauses.
- JOIN Parsing: Correctly parses qualified column names and join conditions.

 4. Operators
These are the building blocks for query execution, each implementing the `Operator` interface:
- TableScanOperator: Iterates over all rows in a table.
- IndexScanOperator: Iterates over rows matching an index lookup.
- SelectionOperator: Filters rows based on a condition.
- ProjectionOperator: Selects specific columns from rows.
- AggregateOperator: Computes aggregation (`AVG`, `SUM`, `COUNT`) over a column.
- NestedLoopJoinOperator: Implements a nested loop join between two tables.

 5. IndexManager
- Purpose: Builds and queries simple indexes for fast lookups.
- Usage: Used in the planner to optimize WHERE queries.

 6. QueryPlanner
- Purpose: Converts the AST into a tree of Operators.
- Features: Handles JOINs, WHERE clauses (with index optimization), projection, and aggregation.

 7. ExecutionEngine
- Purpose: Executes the operator tree and prints results to the console.

 8. TransactionManager
- Purpose: Supports basic transactions (`begin`, `commit`, `rollback`).
- Mechanism: Buffers inserts during a transaction and applies or discards them on commit/rollback.

 9. Database
- Purpose: Main interface for creating tables, inserting data, creating indexes, executing queries, and managing transactions.
- Composition: Holds all tables and managers.

 10. Main Function
- Demonstrates:
    - Table creation and population.
    - Index creation.
    - SELECT queries with WHERE.
    - JOIN queries.
    - Aggregation queries.
    - Transaction demo (insert, rollback).

Code arrangement follows this orde:
The code can be arranged in the following logical order for clarity and maintainability:

1. Includes and Usings
2. Enums and Data Structures
   - NodeType
   - ASTNode
   - Value, Row, Table, TableSchema, Index
3. Operator Interface
4. SQLParser
5. Operators
   - TableScanOperator
   - IndexScanOperator
   - SelectionOperator
   - ProjectionOperator
   - AggregateOperator
   - NestedLoopJoinOperator
6. IndexManager
7. QueryPlanner
8. ExecutionEngine
9. TransactionManager
10. Database
11. Main Function

This order matches the logical flow:
- Data definitions first,
- Then parsing,
- Then execution primitives (operators),
- Then management classes,
- Then the main entry point.

---

 How It Works

1. Table Creation & Data Insertion
   - Tables are created and populated with sample data.

2. Index Creation
   - An index is created on the `students.id` column for fast lookup.

3. Query Execution
   - SQL queries are parsed into an AST.
   - The planner builds an operator tree based on the AST.
   - The execution engine runs the operator tree, printing results.

4. Supported Queries
   - Simple SELECT: `SELECT name, gpa FROM students WHERE id = 2`
   - WHERE with comparison: `SELECT name FROM students WHERE gpa > 3.4`
   - JOIN: `SELECT students.name, courses.course_name FROM students JOIN courses ON students.id = courses.student_id`
   - Aggregation: `SELECT AVG(gpa) FROM students`

5. Transactions
   - Demonstrates inserting a row in a transaction, querying before commit, and rolling back.

---

 Example Output

```
Query 1: SELECT name, gpa FROM students WHERE id = 2
gpa: 3.5        name: Bob

Query 2: SELECT name FROM students WHERE gpa > 3.4
name: Alice
name: Bob
name: Charlie

Query 3: JOIN students and courses on students.id = courses.student_id
courses.course_name: Math       students.name: Alice
courses.course_name: Chemistry  students.name: Alice
courses.course_name: Physics    students.name: Bob

Query 4: Aggregation - SELECT AVG(gpa) FROM students
AVG: 3.5

Transaction Demo:
Inserted Diana, but not committed yet. Querying students:
name: Alice
name: Bob
name: Charlie
Rolled back. Querying students again:
name: Alice
name: Bob
name: Charlie
```

---

 Summary

- What is it?
  A simple, extensible SQL engine for in-memory tables in C++.

- How does it work?  
  It parses SQL, builds an execution plan, and runs queries using composable operator classes.

- What can you do with it?  
  Run basic SQL queries, joins, aggregations, and transactions on in-memory data.

You can extend it to support more SQL features, data types, and optimizations as needed.
