# Dining Philosophers  

This project is a multithreaded simulation of the classical **Dining Philosophers Problem**, implemented in C. The simulation explores concurrency, synchronization, and deadlock prevention in a shared resource environment.  
![](https://github.com/owrmille/philosophers/blob/main/dining_philosophers.png)
## Key Features  
- **Philosophers Lifecycle**: Each philosopher alternates between thinking, eating, and sleeping.  
- **Resource Management**: Forks are represented as shared resources protected by mutexes.  
- **Thread Synchronization**: Uses pthreads for concurrency and ensures proper synchronization to avoid race conditions.  
- **Deadlock Prevention**: Implements strategies to prevent deadlocks, such as consistent resource acquisition ordering.  
- **Configurable Input**: Accepts customizable parameters for the number of philosophers, time to die, time to eat, time to sleep, and the number of meals.  

## How to Use  
1. Clone the repository:  
   ```bash  
   git clone https://github.com/owrmille/philosophers.git   
   ```  

2. Build the project:  
   ```bash  
   make  
   ```  

3. Run the simulation:  
   ```bash  
   ./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]  
   ```  

   Example:  
   ```bash  
   ./philosophers 4 310 200 100  
   ```  

## Project Goals  
- **Understand Multithreading**: Experiment with thread creation and synchronization in C.  
- **Explore Concurrency Issues**: Analyze deadlocks, starvation, and race conditions.  
- **Simulate Real-World Scenarios**: Apply the principles of resource sharing to a simplified real-world scenario.  
