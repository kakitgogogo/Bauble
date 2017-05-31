CREATE DATABASE IF NOT EXISTS teaching DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

USE teaching;

CREATE TABLE stu (
	id int not null auto_increment primary key,
	num varchar(20),
	name varchar(20),
	age int not null default 0,
	unique index(num)
);

CREATE TABLE course (
	id int not null auto_increment primary key,
	num varchar(20),
	name varchar(20)
);

CREATE TABLE transcript (
	id int not null auto_increment primary key,
	courseId int,
	stuId int,
	score int
);
