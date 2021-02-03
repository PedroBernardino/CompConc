/* Disciplina: Computacao Concorrente */
/* Aluno: Pedro Henrique */
/* Codigo: Produtores e Consumidores de Buffer em Java */


//Monitor - Classe do Buffer
class Buffer {
    int[] buffer; // buffer
    int nums = 0; // quantidade de números produzidos e não retirados do buffer -> se 0, vazio,se N-> cheio
    int proxProd = 0; // próxima posição do buffer a ser produzida
    int proxCons = 0; // próxima posição do buffer a ser consumida
    int N; //tamanho do buffer

    //construtor do buffer
    Buffer(int N)
    {
        this.N = N;
        this.buffer = new int[N];
    }

    //imprime o buffer na tela
    public synchronized void imprimeBuffer()
    {
        System.out.print("Buffer: ");
        for(int i = 0; i<N; i++)
        {
            System.out.print(buffer[i]+ " ");
        }
        System.out.println();
    }

    //função da thread produtora, insere um número no buffer
    public synchronized void produzir(int id)
    {
        try{
            System.out.println("Produtor "+id+ " quer produzir");
            while (this.nums == N)
            {
                System.out.println("Produtor "+id+ " Bloqueado");
                wait();
                System.out.println("Produtor "+id+ " Desbloqueado");
            }
            buffer[proxProd] = (id+1);
            proxProd = (proxProd + 1)%N;
            nums++;
            System.out.println("Produtor "+id+" produziu");
            imprimeBuffer();
            notifyAll();
        } catch (InterruptedException e) { }
    }


    //função da thread consumidora, retira um número do buffer
    public synchronized void consumir(int id)
    {
        try{
            System.out.println("Consumidor "+id+ " quer consumir");
            while (this.nums == 0)
            {
                System.out.println("Consumidor "+id+ " Bloqueado");
                wait();
                System.out.println("Consumidor "+id+ " Desbloqueado");
            }
            int retirado = buffer[proxCons];
            buffer[proxCons] = 0;
            nums--;
            proxCons = (proxCons+1)%N;
            System.out.println("Consumidor "+id+" consumiu o número "+retirado);
            imprimeBuffer();
            notifyAll();
        } catch (InterruptedException e) { }
    }
}

//Thread produtora
class Produtor extends Thread {
    int id;
    int delay;
    Buffer buffer;

    Produtor (int id, int delayTime, Buffer buffer)
    {
        this.id = id;
        this.delay = delayTime;
        this.buffer = buffer;
    }

    public void run() {
        try {
            for(;;) {
                buffer.produzir(id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

//Thread Consumidora
class Consumidor extends Thread {
    int id;
    int delay;
    Buffer buffer;

    Consumidor (int id, int delayTime, Buffer buffer)
    {
        this.id = id;
        this.delay = delayTime;
        this.buffer = buffer;
    }

    public void run() {
        try {
            for(;;) {
                buffer.consumir(id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

class ProdutorConsumidor {
    static final int P = 3; //Número de Threads Produtoras
    static final int C = 3; //Número de Threads Consumidoras
    static final int N = 5; //Tamanho do Buffer

    public static void main(String[] args) {
        //Inicia o buffer e os conjuntos de Threads
        Buffer buffer = new Buffer(N);
        Produtor[] p = new Produtor[P];
        Consumidor[] c = new Consumidor[C];


        //Cria e starta as Threads
        for (int i = 0; i < P; i++) {
            p[i] = new Produtor(i, 1, buffer);
            p[i].start();
        }

        for (int i = 0; i < C; i++) {
            c[i] = new Consumidor(i, 1, buffer);
            c[i].start();
        }
    }
}