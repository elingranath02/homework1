import java.util.Random;
import java.lang.*;


import sharedbathroom.SharedBathroom;

public class Threads extends Thread {

    long maxSleep = 5000;
    long maxSleepBathroom = 3000;

    SharedBathroom bathroom = new SharedBathroom();

    public Threads() {

    }

    public static void main(String[] args) {

        int maximum_threads = 6;
        for (int i = 0; i < maximum_threads; i++) {
            Threads thread = new Threads();
            thread.start();
        }

        // System.out.println("This code is outside of the thread");
    }

    public void threadSleep(long sleepTime) {

        Random random = new Random();
        long sleep = 0;
        while (sleep == 0) {
            sleep = random.nextLong(sleepTime);
        }
        try {
            Thread.sleep(sleep);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void run() {
        while (true) {

            threadSleep(maxSleep);

            if ((Thread.currentThread().getId()) % 2 == 0) {
                womanQueue();
            } else {
                manQueue();
            }
        }
    }
    // System.out.println("Current thread id: " +
    // Thread.currentThread().threadId());

    public void womanQueue() {

        System.out.println("Woman: " + Thread.currentThread().getId() + " in queue");
        bathroom.womanEnter();
        threadSleep(maxSleepBathroom);
        bathroom.womenExit();

    }

    public void manQueue() {
        System.out.println("Man: " + Thread.currentThread().getId() + " in queue");
        bathroom.manEnter();
        threadSleep(maxSleepBathroom);
        bathroom.manExit();

    }
}