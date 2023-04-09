public class NewPayment implements PayManager {
    @Override
    public int pay(int workHours, int overTimeHours) {
        if (overTimeHours>10)
            return 20000*workHours+30000*overTimeHours;
        else
            return 10000*workHours+20000*overTimeHours;
    }
}
