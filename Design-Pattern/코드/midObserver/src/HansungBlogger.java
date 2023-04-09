public class HansungBlogger extends Blogger {
    private int ostate, nstate;

    public int getState() {
        return nstate;
    }
    public void setState(int state) {
        this.ostate = nstate;
        this.nstate = state;
        if (this.ostate != this.nstate) broadcast();
    }
}