import java.util.ArrayList;
import java.util.List;


abstract class Report{
    public abstract void displayContent();
}

class BasicReport extends Report{
    private  String msg;

    public BasicReport(String msg){
        this.msg = msg;
    }

    @Override
    public void displayContent() {
        System.out.println(msg);
    }
}

class ReportAdded extends Report{
    private Report report;

    public ReportAdded(Report report){
        this.report = report;
    }

    @Override
    public void displayContent() {
        report.displayContent();
    }
}

class Star extends ReportAdded {
    public Star(Report report){
        super(report);
    }

    public void displayContent(){
        displayStar();
        super.displayContent();
        displayStar();
    }

    private void displayStar(){
        System.out.println("**********");
    }

}

class Sharp extends ReportAdded {
    public Sharp(Report report){
        super(report);
    }
    public void displayContent(){
        displayStar();
        super.displayContent();
        displayStar();
    }

    private void displayStar(){
        System.out.println("##########");
    }

}





public class Main {
    public static void main(String[] args) {
        HansungBlogger h1 = new HansungBlogger();
        HansungBlogger h2 = new HansungBlogger();
        Subscriber m1 = new BinarySubscriber(h1);
        Subscriber m2 = new HexaSubscriber(h2);
        Subscriber m3 = new OctalSubscriber(h1);

        h1.join(m1);
        h2.join(m2);
        h1.join(m3);

        h1.setState(10);
        h2.setState(5);
        h1.setState(10);

        Report r = new Sharp(new Star(new BasicReport("Design Pattern")));
        r.displayContent();

    }
}