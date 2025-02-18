package sharedbathroom;

public class SharedBathroom {

    private volatile boolean womenTurn;
    private volatile boolean menTurn;
    private volatile int nrOfWomenInBathroom;
    private volatile int nrOfMenInBathroom;
    private volatile int nrOfWomenInQueue;
    private volatile int nrOfMenInQueue;

    public SharedBathroom() {
        this.womenTurn = false;
        this.menTurn = false;
        this.nrOfWomenInBathroom = 0;
        this.nrOfMenInBathroom = 0;
        this.nrOfWomenInQueue = 0;
        this.nrOfMenInQueue = 0;
    }

    
    public synchronized void womanEnter() {
        if(nrOfMenInQueue == 0){
        womenTurn = true;
        }
       // System.out.println("Value of womenlock: " + getWomenTurn());
        while (menTurn) {
            try {
                nrOfWomenInQueue++;
                //System.out.println("Woman: " + Thread.currentThread().threadId() + " waiting");
                
                wait();
                nrOfWomenInQueue--;

            } catch (InterruptedException e) {
            }
           
            womenTurn = true;
        }
        
        //System.out.println("Value of womenlock 2: " + getWomenTurn());
        nrOfWomenInBathroom++;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " enters bathroom");
        
    }

    public synchronized void manEnter() {
        if(nrOfWomenInQueue == 0){
        menTurn = true;
        }
        
        //System.out.println("Value of manlock: " + getMenTurn());
        while (womenTurn) {
            try {
                //System.out.println("Man: " + Thread.currentThread().threadId() + " waiting");
                nrOfMenInQueue++;
                
                    wait();
                
                    nrOfMenInQueue--;
            } catch (InterruptedException e) {
            }
            menTurn = true;
        
        }
        nrOfMenInBathroom++;
        System.out.println("Man: " + Thread.currentThread().threadId() + " enters bathroom");
       
    }

    public synchronized void womenExit() {
        nrOfWomenInBathroom--;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " exits bathroom");

        if (nrOfWomenInBathroom == 0){
           
            womenTurn = false;
            //menTurn = true;
            System.out.println("Women in queue: " + nrOfWomenInQueue);
            notifyAll();

            //System.out.println("Value of menlock: " + getWomenTurn());
            System.out.println();
            System.out.println("Nr of women in bathroom: " + nrOfWomenInBathroom);
            System.out.println();
        }
    }

    public synchronized void manExit() {
        nrOfMenInBathroom--;
        System.out.println("Man: " + Thread.currentThread().threadId() + " exits bathroom");
        
        if (nrOfMenInBathroom == 0) {
            menTurn = false;
            //womenTurn = true;
            System.out.println("Men in queue: " + nrOfMenInQueue);
            notifyAll();
            
            System.out.println();
            System.out.println("Nr of men in bathroom: " + nrOfMenInBathroom);
            System.out.println();
            
         }
    }

}

