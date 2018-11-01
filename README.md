[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
# Neighbors problem: nearest and radius neighbors
<p align="center">
<img src="https://www.researchgate.net/profile/Saleh_Alaliyat/publication/267953942/figure/fig14/AS:295388776026147@1447437580523/K-nearest-neighbor-algorithm-illustration-The-green-circle-is-the-sample-which-is-to-be.png" width="470px" height="250px"> <br /> <br />
Available methods: 
* Locality-sensitive hashing(lsh)
* Hypercube
* Exhuastive search
Available metrices: euclidean and cosine
</p>
# Requirements
* Compiler: g++ - C11 standard

# Installation
* Clone this repo to your local machine: 
```
git clone https://github.com/PetropoulakisPanagiotis/neighbors-problem.git
```
# Tutorial
* This repository contains a [tutorial](https://github.com/PetropoulakisPanagiotis/neighbors-problem/tree/master/tutorial) for the newcomers

# How It Works
* Lsh: Each model stores the data points in L hash tables. Every new query is hashed and is <br />  compared with the data of one specific bucket
* Hypercube: Each model stores the data points in a hypercube structure(vector). Every new query is hashed and is <br /> compared with the data of one or more(neighbor vertices) vertices

## Hash functions: Lsh Euclidean
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/h_euclidean.png) <br /> <br />
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/hash_table_euclidean.png)

## Hash functions: Lsh Cosine
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/h_cosin.png) <br /> <br />
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/lsh_cosin.png)

## Hash functions: Hypercube
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/hypercube.png)

# The hyperparameters
* L(lsh): Number of hash tables
* K: Number of sub hash functions
* W(euclidean): Window size. The value depends on the anticipate distance from the nearest neighbor
* Coefficient(euclidean lsh): Defines the table size of each hash table. Table size = n(number of data points) * coefficient(< 1)

## Αcknowledgements
Thank you Ioannis z. Emiris Professor @ Department of Informatics & Telecommunications <br />
National and Kapodistrian University of Athens that let me to use his slides<br />
Webpage: http://cgi.di.uoa.gr/~emiris/index-eng.html

## Author
Petropoulakis Panagiotis petropoulakispanagiotis@gmail.com
