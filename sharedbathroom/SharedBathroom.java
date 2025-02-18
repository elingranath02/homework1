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

    
    public synchronized void womanEnter() {
        
        setWomenTurn(true);
        
       // System.out.println("Value of womenlock: " + getWomenTurn());
       
        while (getMenTurn()) {
            try {
                nrOfWomenInQueue++;
                //System.out.println("Woman: " + Thread.currentThread().threadId() + " waiting");
                wait();
            } catch (InterruptedException e) {
            }
            setWomenTurn(true);
        }
        
        //System.out.println("Value of womenlock 2: " + getWomenTurn());
        nrOfWomenInQueue--;
        increaseWomen();
        System.out.println("Woman: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void manEnter() {
        setMenTurn(true);
        
        //System.out.println("Value of manlock: " + getMenTurn());
        while (getWomenTurn()) {
            try {
                //System.out.println("Man: " + Thread.currentThread().threadId() + " waiting");
                nrOfMenInQueue++;
                wait();
            } catch (InterruptedException e) {
            }
            setMenTurn(true);
        }
        nrOfMenInQueue--;
        increaseMen();
        System.out.println("Man: " + Thread.currentThread().threadId() + " enters bathroom");
    }

    public synchronized void womenExit() {
        decreaseWomen();
        System.out.println("Woman: " + Thread.currentThread().threadId() + " exits bathroom");

        if (getNrOfWomenInBathroom () == 0){
           
            setWomenTurn(false);
            notifyAll();
            //System.out.println("Value of menlock: " + getWomenTurn());
            System.out.println();
            System.out.println("Nr of women in bathroom: " + getNrOfWomenInBathroom());
            System.out.println();
            

        }
    }

    public synchronized void manExit() {
        decreaseMen();
        System.out.println("Man: " + Thread.currentThread().threadId() + " exits bathroom");
        
        if (getNrOfMenInBathroom() == 0) {
            setMenTurn(false);
            notifyAll();
            System.out.println();
            System.out.println("Nr of men in bathroom: " + getNrOfMenInBathroom());
            System.out.println();
            
         }
    }

}