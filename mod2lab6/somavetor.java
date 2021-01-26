/* Disciplina: Computacao Concorrente */
/* Aluno: Pedro Henrique Bernardino */
/* Módulo 2 - Laboratório 6 */
/* Soma de vetores usando computação concorrente */

import java.util.Random;




//classe da Thread
class SomaVetor extends Thread {
    private int id;
    int N; // número de Threads
    int TAM; // Tamanho dos vetores;
    int[] vet1; //primeiro vetor da soma
    int[] vet2; //segundo vetor da soma
    int[] result; //vetor resultado da soma

    //--construtor - Thread recebe seu id
    public SomaVetor(int id, int N, int TAM, int[] vet1, int[] vet2, int[] result) {
        this.id = id;
        this.N = N;
        this.TAM = TAM;
        this.vet1 = vet1;
        this.vet2 = vet2;
        this.result  = result;
    }

    //--metodo executado pela thread
    public void run() {
        for(int i = id; i< TAM; i+= N)
        {
            result[i] = vet1[i] + vet2[i];
        }
    }
}

//classe principal do programa, que gerencia as threads
class Main
{


    public static void main(String[] args) {

        int N = 4; // número de Threads
        int TAM = 10; // Tamanho dos vetores;
        int[] vet1 = new int[TAM]; //primeiro vetor da soma
        int[] vet2 = new int[TAM]; //segundo vetor da soma
        int[] result = new int[TAM]; //vetor resultado da soma

        Thread[] threads = new Thread[N];

        //cria as threads
        for (int i=0; i<threads.length; i++) {
            threads[i] = new SomaVetor(i, N, TAM, vet1, vet2, result);
        }

        //inicializa os vetores
        for(int i=0; i<TAM; i++) {
            vet1[i] = new Random().nextInt(100);
            vet2[i] = new Random().nextInt(100);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //espera as threads encerrarem
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); }
            catch (InterruptedException e) { return; }
        }


        //exibição dos resultados
        for(int i=0; i<TAM; i++) {
            System.out.print(vet1[i] + " ");
        }
        System.out.println("");
        for(int i=0; i<TAM; i++) {
            System.out.print(vet2[i] + " ");
        }
        System.out.println("");
        for(int i=0; i<TAM; i++) {
            System.out.print(result[i] + " ");
        }
    }
}