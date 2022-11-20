# Multithreading Recommendation Tetris with ncurses (MRT)


## 1. (Basic) Tetris
1) Rank system
- Sorted Linked List
![image](https://user-images.githubusercontent.com/81512075/202933506-a3ab300a-3489-467d-b53e-dd3e28ec0647.png)

2) Shadow block

3) Greedy Recommendation Algorithm
- `./a.out {REC_CAL}` , Default REC_CAL = 4
- criteria 
    (1) : How many Line the choice can break
    (2) : If (1) not, for distrubition of height of block, coordinate Y value is criteria
- using only single core, would be slow
## 2. Multithreading Recommendation Tetris

#### 1) Leverage multithreading :
    - 1 Main tetris thread, 4 (==NUM_OF_ROTATE) Calculation Thread(c-thread) work at background
    - c-thread wait for worker_mutex[thread_identifier], run after at block 'Freezed'.
    - Each c-thread calcuate recursively for different rotation of first block.
    - After c-thread calcuation complete, woke up main thread(sem_post(&global_mutex))
    
![image](https://user-images.githubusercontent.com/81512075/202933463-1710f32c-e4bf-4a1d-a55f-32a3a86725e9.png)
