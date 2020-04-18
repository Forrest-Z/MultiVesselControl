#!/bin/bash



sqlite3 ./estimator.db <<!
.headers on
.mode csv
.output state.csv
select * from state;
.output stdout

.output error.csv
select * from error;
.output stdout

.output measurement.csv
select * from measurement;
.output stdout


