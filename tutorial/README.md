# Welcome to neighbors problem tutorial
* Step 1: Create the data points - Header files: item.h, utils.h
```
list<Item> dataPoints;
errorCode status; // Keep errors
for(i = 0; i < n; i++){
  vector<double> components;
  
  for(j = 0; j < dim; j++){
    components.push_back(i + 5);
  }
  
  dataPoints.push_back(Item(components), status);
  if(status != SUCCESS){
    printError(status);
    return;
  }
}
```

* Step 2: Pick a model - Header files: model.h and lshEuclidean.h or lshCosin.h or hypercubeEuclidean.h or hypercubeCosin.h
```
model* myModel;
myModel = new lshEuclidean();
```
or you can pick your favorable hyperparameters
```
int l = 7, coefficient = 0.2, k = 9, w = 200;
myMode = new lshEuclidean(coefficient, l, k, w);
```

* Step 3: Train model with the data ponts
```
myModel->fit(dataPoints, status);
if(status != SUCCESS){
  printError(status);
  delete myModel
  return;
}
```

* Step 4: Find radius and nearest neighbors for a query point
```
vector<double> queryComponents;
for(i = 0; i < dim; i++)
  queryComponents.push_back(i + 13);
  
Item query(queryComponents, status);
if(status != SUCCESS){
  printError(status);
  delete myModel
  return;
}

int radius;
list<double> neighborsDistances;
list<Item> neighbors;
double nearestDistance;
Item nearestNeighbor;

/* Find radius */
myModel->radiusNeighbors(query, radius, neighbors, neighborsDistances, status);
if(status != SUCCESS){
  printError(status);
  delete myModel
  return;
}

/* Find nearest neighbor */
myModel->nNeighbor(query, nearestNeighbor, nearestDistance, status);
if(status != SUCCESS){
  printError(status);
  delete myModel
  return;
}
```

* Step 5: Print neighbors - See item.h for more accessors 
```
list<Item>::iterator iter;
list<double>::iterator distances = neighborsDistances.begin(); 

for(iter = neighbors.begin(); iter != neighbors.end(); iter++){
  cout << Neighbor id: << iter->getId();
  cout << Neighbor dist: << *distances << "\n";
  
  distances++;
}

cout << Nearest neighbor id: << nearestNeighbor.getId();
cout << Nearest neighbor dist: << nearestDistance << "\n";
```

* Step 6: Compilation - Take a look in experiments makefiles 
