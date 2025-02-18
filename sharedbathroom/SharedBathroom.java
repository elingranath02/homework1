package sharedbathroom;

public class SharedBathroom {

    private boolean womenTurn;
    private boolean menTurn;
    private int nrOfWomenInBathroom;
    private int nrOfMenInBathroom;

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
        // womenTurn = true;
        setWomenTurn(true);

        System.out.println("Value of lock: " + womenTurn);
        System.out.println("Nr of women in bathr: " + getNrOfWomenInBathroom());
        if (getMenTurn() == true) {
            System.out.println("bajs");
        }
        while (getMenTurn()) {
            System.out.println("Woman: " + Thread.currentThread().getId() + " waiting");
            try {
                wait();
            } catch (InterruptedException e) {
            }
        }
        increaseWomen();
        System.out.println("Woman: " + Thread.currentThread().getId() + " enters bathroom");

    }

    public synchronized void manEnter() {
        setMenTurn(true);

        System.out.println("Value of lock: " + menTurn);
        System.out.println("Nr of women in bathroom: " + getNrOfWomenInBathroom());
        if (getWomenTurn() == true) {
            System.out.println("bajs");
        }
        while (getWomenTurn()) {
            System.out.println("Man: " + Thread.currentThread().getId() + " waiting");
            try {
                wait();
            } catch (InterruptedException e) {
            }
        }
        increaseMen();
        System.out.println("Man: " + Thread.currentThread().getId() + " enters bathroom");
    }

    public synchronized void womenExit() {
        decreaseWomen();
        System.out.println("Woman: " + Thread.currentThread().getId() + " exits bathroom");

        if (getNrOfWomenInBathroom() == 0) {
            setWomenTurn(false);
            notifyAll();
        }

    }

    public synchronized void manExit() {
        decreaseMen();
        System.out.println("Man: " + Thread.currentThread().getId() + " exits bathroom");
        if (getNrOfMenInBathroom() == 0) {
            setMenTurn(false);
        }
    }

}