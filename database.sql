-- Create the database
CREATE DATABASE IF NOT EXISTS data;

-- Use the database
USE data;

-- Create the table
CREATE TABLE IF NOT EXISTS distancedata
(
    id        INT AUTO_INCREMENT PRIMARY KEY,
    distance  INT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP NULL
);
