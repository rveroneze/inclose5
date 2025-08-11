# In-Close5

In-Close5 is a fast formal concept miner

Author's implementation: https://sourceforge.net/projects/inclose/

Related works:
- Andrews, S. (2018). A new method for inheriting canonicity test failures in close-by-one type algorithms.
- Andrews, S. (2017). Making use of empty intersections to improve the performance of cbo-type algorithms. In International Conference on Formal Concept Analysis (pp. 56-71).
- Andrews, S. (2015). A ‘Best-of-Breed’approach for designing a fast algorithm for computing fixpoints of Galois Connections. Information Sciences, 295, 633-649.
- Andrews, S. (2011). In-close2, a high performance formal concept miner. In International Conference on Conceptual Structures (pp. 50-62).
- Andrews, S. (2009). In-close, a fast algorithm for computing formal concepts.


## === Experimental branch for feature testing ===


## Compiling
To compile the program, run the file  './MakeFile.sh' in the directory containing the source files.


## Running
To run the program, type './InClose5' and the arguments:

1) Dataset's filename;

2) minimum number of rows;

3) minimum number of columns;

4) Output filename for the list of biclusters;

5) Class labels' filename;

6) Minimum ZDC value [0,1];

For instance:
./InClose5 exampleDataset 2 1 exampleOutput.m exampleLabels 0.95

### Note:
The code in this branch performs the same operations as the master branch, but with an additional supervised filtering step on the output. This filter evaluates each pattern (formal concept) based on the χ2 test, which is a zero diagonal convex (ZDC) function. Only patterns meeting the minimum χ2 threshold are retained.

The χ2 test implementation is based on the paper:
- Nijssen, S., & Kok, J. N. (2005, October). Multi-class correlated pattern mining. In International Workshop on Knowledge Discovery in Inductive Databases (pp. 165-187). Berlin, Heidelberg: Springer Berlin Heidelberg.

In this paper, the authors state: "As for a database of size N the highest achievable χ2 value is N, we will choose χ2 thresholds which are percentages of N." 

Following this approach, we divide the χ2 statistic by N.
