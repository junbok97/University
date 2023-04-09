class Counter {
    private  volatile static Counter instance;
    private int count;
    private Counter() { count = 0;}
    public synchronized void enter() { count++;}
    public synchronized void exit() { count--;}
    public static Counter getInstance() {
        if (instance == null) {
            synchronized (Counter.class) {
                if (instance == null) instance = new Counter();
            }
        }
        return instance;
    }

    public int getCount() {
        return count;
    }
}