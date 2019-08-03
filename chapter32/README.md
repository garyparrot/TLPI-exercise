# Chapter 32

## Async Cancel 

Async cancellation can be perform on anywhere in function.

Better don't do this with function who accessing mutex or allocating memory.

That might broken something if the function terminated in the middle of important works.

