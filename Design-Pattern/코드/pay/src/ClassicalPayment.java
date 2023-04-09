public class ClassicalPayment implements PayManager {
    @Override
    public int pay(int workHours, int overTimeHours) {
        return 10000*workHours+15000*overTimeHours;
    }
}
