import java.lang.Object;
import java.lang.Thread;

public class Threads extends Thread {
    public static void main(String[] args) {

        int maximun_threads = 5;
        for (int i = 0; i < maximun_threads; i++) {
            Threads thread = new Threads();
            thread.start();
        }

        // System.out.println("This code is outside of the thread");
    }

    public void run() {
        System.out.println("Current thread name: " + Thread.currentThread().getName());
        System.out.println("Current thread id: " + Thread.currentThread().threadId());
    }
}
