package sharedbathroom;

public class SharedBathroom2 {

    private volatile boolean womenTurn;
    private volatile boolean menTurn;
    private volatile int nrOfWomenInBathroom;
    private volatile int nrOfMenInBathroom;
    private volatile int nrOfWomenInQueue;
    private volatile int nrOfMenInQueue;

    public SharedBathroom2() {
        this.womenTurn = false;
        this.menTurn = false;
        this.nrOfWomenInBathroom = 0;
        this.nrOfMenInBathroom = 0;
    }

    private void setWomenTurn(boolean value) {
        this.womenTurn = value;
    }

    private void setMenTurn(boolean value) {
        this.menTurn = value;
    }

    private void increaseWomen() {
        this.nrOfWomenInBathroom++;
    }

    private void increaseMen() {
        this.nrOfMenInBathroom++;
    }

    private void decreaseWomen() {
        this.nrOfWomenInBathroom--;

    }

    private void decreaseMen() {
        this.nrOfMenInBathroom--;
    }

        private void increaseWomenQueue() {
        this.nrOfWomenInQueue++;
    }

    private void increaseMenQueue() {
        this.nrOfMenInQueue++;
    }

    private void decreaseWomenQueue() {
        this.nrOfWomenInQueue--;

    }

    private void decreaseMenQueue() {
        this.nrOfMenInQueue--;
    }

    private boolean getWomenTurn(){
        return this.womenTurn;
        
    }

    private boolean getMenTurn(){
        return this.menTurn;
    }

    private int getNrOfWomenInBathroom(){
        return this.nrOfWomenInBathroom;
    }

    private int getNrOfMenInBathroom(){
        return this.nrOfMenInBathroom;
    }

    private int getNrOfWomenInQueue(){
        return this.nrOfWomenInQueue;
    }

    private int getNrOfMenInQueue(){
        return this.nrOfMenInQueue;
    }

    
    public synchronized void womanEnter() {
        
       // System.out.println("Value of womenlock: " + getWomenTurn());
       setWomenTurn(true);
       increaseWomenQueue();
       
        while (getMenTurn()) {
            try {
                //System.out.println("Woman: " + Thread.currentThread().threadId() + " waiting");
                wait();
            } catch (InterruptedException e) {
            }
        }
        //System.out.println("Value of womenlock 2: " + getWomenTurn());
        decreaseWomenQueue();
        increaseWomen();
        System.out.println("Woman: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void manEnter() {
        setMenTurn(true);
        increaseMenQueue();
        //System.out.println("Value of manlock: " + getMenTurn());
        while (getWomenTurn()) {
            try {
                //System.out.println("Man: " + Thread.currentThread().threadId() + " waiting");
                wait();
            } catch (InterruptedException e) {
            }
        }
        decreaseMenQueue();
        increaseMen();
        System.out.println("Man: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void womenExit() {
        decreaseWomen();
        System.out.println("Woman: " + Thread.currentThread().threadId() + " exits bathroom");

        if (getNrOfWomenInBathroom () == 0 && getNrOfWomenInQueue() ==0){
           
            setWomenTurn(false);
            notifyAll();
            //System.out.println("Value of menlock: " + getWomenTurn());
            System.out.println();
            System.out.println("Nr of women in bathroom: " + getNrOfWomenInBathroom());
            System.out.println();
            

        }
        else {
            notifyAll();
        }
    }

    public synchronized void manExit() {
        decreaseMen();
        System.out.println("Man: " + Thread.currentThread().threadId() + " exits bathroom");
        
        if (getNrOfMenInBathroom() == 0 && getNrOfMenInQueue() == 0) {
            setMenTurn(false);
            notifyAll();
            System.out.println();
            System.out.println("Nr of men in bathroom: " + getNrOfMenInBathroom());
            System.out.println();
            
         }
         else {
            notifyAll();
         }
    }

}