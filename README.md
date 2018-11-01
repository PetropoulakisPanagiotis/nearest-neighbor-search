[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
# Neighbors problem: nearest and radius neighbors
<img src="https://www.researchgate.net/profile/Saleh_Alaliyat/publication/267953942/figure/fig14/AS:295388776026147@1447437580523/K-nearest-neighbor-algorithm-illustration-The-green-circle-is-the-sample-which-is-to-be.png" width="500px" height="250px"> <br />
Available methods: 
* Lsh Euclidean / Cosine
* Hypercube Euclidena / Cosine
* Exhuastive search

# How It Works
* Lsh: Each model stores the data points in L hash tables. Every new query is hashed and it is <br />  compared with the data of one specific bucket 
* Hypercube: Each model stores the data points in a hypercube. Every new query is hashed and it is <br /> compared with the data of one or more(neighbor vertices) vertices.
## Hash functions: Lsh Euclidean
Sub hash functions:
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/h_euclidean.png)
Main hash function:
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/hash_table_euclidean.png)

## Hash functions: Lsh Cosine
Sub hash functons: 
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/h_cosin.png)
Main hash function:
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/lsh_cosin.png)
## Hash functions: Hypercube
![alt text](https://github.com/PetropoulakisPanagiotis/neighbors-problem/blob/master/images/hypercube.png)
# Tutorial 

## Requirements
* Compiler: g++ - C11 standard

## Αcknowledgements
Thank you Ioannis z. Emiris Professor @ Department of Informatics & Telecommunications <br />
National and Kapodistrian University of Athens that let me to use his slides.<br />
Webpage: http://cgi.di.uoa.gr/~emiris/index-eng.html
## Author
Petropoulakis Panagiotis petropoulakispanagiotis@gmail.com
