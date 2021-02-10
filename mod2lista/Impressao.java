import java.util.Queue;
import java.util.LinkedList;

class Impressao {
    int id;
    Object documento;

    Impressao(int id, Object documento) {
        this.id = id;
        this.documento = documento;
    }

    public void printPedido() {
        System.out.println("Pedido n° " +id);
    }
}




// Monitor
class Impressora {
    //numero maximo de threads fila para acessar a fila de impressão
    static final int N = 50;
    //atributos de estado da execucao
    private int qtdFila; //qtde de threads na fila de impressão
    private int ocupado; //estado da impressora (0: desocupado; 1: ocupado)
    private int proximaThread; //numero/senha da proxima thread que deve ser atendida
    private int ultimaThread; //numero/senha da proxima thread a entrar na fila de espera
    private Queue<Impressao> fila; // fila de Impressão
    // Construtor
    Impressora() {
        this.qtdFila = 0; //(no maximo N=50)
        this.ocupado = 0;
        this.proximaThread = 0;
        this.ultimaThread = 0;
        fila = new LinkedList<>();
    }
    // Metodo das threads
    public synchronized boolean AcessoFila(Object documento) {
        if(this.qtdFila == N) {
            return false; //a thread nao consegue entrar na Fila de impressão
        }
        try {
            int minhaSenha;
            this.qtdFila++;
            minhaSenha = this.ultimaThread;
            this.ultimaThread++;
            Impressao pedido = new Impressao(minhaSenha,documento);
            fila.add(pedido);
            while(this.proximaThread != minhaSenha) this.wait();
            this.ocupado = 1;
            this.notifyAll();
        } catch (InterruptedException e) {return false; }
        return true;
    }
    // Metodo do processador para aguardar por threads prontas
    public synchronized void EsperaThread() {
        try {
            while((this.qtdFila == 0) || (this.ocupado == 0)) this.wait();
            this.qtdFila--;
            Impressao proximo = fila.poll();
        } catch (InterruptedException e) { }
    }
    // Metodo da impressora para sinalizar que terminou a execuç̃ao de uma thread
    public synchronized void TerminaThread() {
        this.ocupado = 0;
        this.proximaThread++;
        if(this.qtdFila > 0) this.notifyAll();
    }

    public synchronized void PrintaFila()
    {
        System.out.println("Lista de Pedidos -");
        for (Impressao pedido:
             fila) {
            pedido.printPedido();
        }
    }
}