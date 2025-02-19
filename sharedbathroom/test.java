package sharedbathroom;

public class test {

    private volatile boolean womenTurn;
    private volatile boolean menTurn;
    private volatile int nrOfWomenInBathroom;
    private volatile int nrOfMenInBathroom;
    private volatile int nrOfWomenInQueue;
    private volatile int nrOfMenInQueue;

    public test() {
        this.womenTurn = false;
        this.menTurn = false;
        this.nrOfWomenInBathroom = 0;
        this.nrOfMenInBathroom = 0;
    }
    
    public synchronized void womanEnter() {
       // System.out.println("Value of womenlock: " + getWomenTurn());
       womenTurn = true;
       nrOfWomenInQueue++;
      
       
        while (menTurn) {
            try {
                //System.out.println("Woman: " + Thread.currentThread().threadId() + " waiting");
                wait();
            } catch (InterruptedException e) {
            }
        }
        //System.out.println("Value of womenlock 2: " + getWomenTurn());
        nrOfWomenInQueue--;
        nrOfWomenInBathroom++;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void manEnter() {
        menTurn = true;
        nrOfMenInQueue++;
        //System.out.println("Value of manlock: " + getMenTurn());
        while (womenTurn) {
            try {
                //System.out.println("Man: " + Thread.currentThread().threadId() + " waiting");
                wait();
            } catch (InterruptedException e) {
            }
        }
        nrOfMenInQueue++;
        nrOfMenInBathroom++;
        System.out.println("Man: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void womenExit() {
        nrOfWomenInBathroom--;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " exits bathroom");

        if (nrOfWomenInBathroom == 0 ){
           
            womenTurn=false;
            notifyAll();
            //System.out.println("Value of menlock: " + getWomenTurn());
            System.out.println();
            System.out.println("Nr of women in bathroom: " + nrOfWomenInBathroom);
            System.out.println();

        }
        else {
            notifyAll();
        }
    }

    public synchronized void manExit() {
        nrOfMenInBathroom--;
        System.out.println("Man: " + Thread.currentThread().threadId() + " exits bathroom");
        
        if (nrOfMenInBathroom == 0 ) {
            menTurn = false;
            notifyAll();
            System.out.println();
            System.out.println("Nr of men in bathroom: " + nrOfMenInBathroom);
            System.out.println();
            
         }
         else {
            notifyAll();
         }
    }

}