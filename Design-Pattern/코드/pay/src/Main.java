public class Main {
    public static void main(String[] args) {
        Employee emp1 = new Employee("1234", "insang1");
        emp1.setWorkHours(40);
        emp1.setOverTimeHours(10);
        emp1.setPayManager(new NewPayment());
        System.out.println(emp1.pay());
    }
}
