public class Decrementor extends Thread {
    private Counter counter;
    public Decrementor(Counter counter) {
        this.counter = counter;
    }
    @Override
    public void run() {
        for (int i = 0; i<=10; i++) {
            try {
                sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            counter.exit();
        }
    }
}
