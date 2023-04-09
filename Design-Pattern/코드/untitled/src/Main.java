abstract class Y {
    public int foo(int n) {
        int total = xmethod();
        for (int i = 1; i <= n; i++) {
            if (ymethod(i))
                total = zmethod(total , i);
        }
        return total;
    }
    protected abstract int xmethod();
    protected abstract boolean ymethod(int x);
    protected abstract int zmethod(int x1, int x2);
}

class X extends Y
{
    @Override
    protected int xmethod() {
        return 0;
    }

    @Override
    protected boolean ymethod(int x1) {
        return  x1%3 ==0; //%연산자만 사용하시오.
    }

    @Override
    protected int zmethod(int x1, int x2) {
        return   x1+x2
        ; //;을 중복되게 넣지 마시오
    }
}

public class Main {
    public static void main(String[] args) {
        Y c1 = new X();
        System.out.print(    c1.foo(10)        );
    }
}