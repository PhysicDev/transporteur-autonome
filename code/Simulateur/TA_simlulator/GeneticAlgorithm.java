package NeuralNet;
import java.util.Arrays;
import java.util.Random;
import java.util.ArrayList;

public class GeneticAlgorithm extends NeuralNetwork {
  Random R= new Random();
  private int popNum;
  public int getPop() {
    return(popNum);
  }

  private int Gen=0;
  public int getGen() {
    return(Gen);
  }
  private NeuralNetwork[] population;
  private int fitness[];
  public void setFit(int value,int id) {
    fitness[id]=value;
  }
  
  public void train(int i,ArrayList<float[][]> dataBase,int tests){
    population[i].backPropagation(dataBase,tests);
  }

  public GeneticAlgorithm(int In, int[] Hid, int Out) throws IllegalArgumentException {
    super(In, Hid, Out);
  }
  public GeneticAlgorithm(int[] Structure) throws IllegalArgumentException {
    super(Structure);
  }

  public void init(int pop) throws ArithmeticException{
    if(pop<3) {
      throw new ArithmeticException("the minimum population size is 3");
    }
    popNum=pop;
    population = new NeuralNetwork[pop];
    fitness=new int[pop];
    for(int i=0;i<pop;i++) {
      population[i]= new NeuralNetwork(structure);
      population[i].apply((NeuralNetwork)this);
      population[i].randomize();
    }
  }

  public int nextGen() {
    int[] Parent=nBest(5); 
    apply(population[Parent[0]]);
    for(int i=0;i<popNum;i++) {
      //we keep the best of the last generation to avoid regression
      if(Parent[0]!=i) {
        population[i].apply(population[Parent[R.nextInt(Parent.length)]]);//population[R.nextInt(population.length)]
        if(Gen>2){
          population[i].crossOver(population[Parent[0]],population[R.nextInt(population.length)] , 0.002f);
        }
        population[i].mutate(0.005f);
      }
    }
    Gen++;
    return(fitness[Parent[0]]);
  }

  public int meanFit(){
    int sum=0;
    for(int i:fitness){
      sum+=i;
    }
    return(sum/popNum);
  }

  private int[] nBest(int n) throws ArithmeticException {
    if(n>popNum) {
      throw new ArithmeticException("Error : n exceded the population size");
    }
    int[] Fitness = Arrays.copyOf(fitness, popNum);
    int[] Out = new int[n];
    int[] Best = new int[n];
    Arrays.fill(Out, -1);
    Arrays.fill(Best, Integer.MIN_VALUE);
    for(int j=0;j<n;j++) {
      for(int i=0;i<popNum;i++) {
        if(Fitness[i]>Best[j]) {
          Out[j]=i;
          Best[j]=Fitness[i];
        }
      }
      Fitness[Out[j]]=Integer.MIN_VALUE;
    }
    return(Out);
  }
  public float[] computeOutput(int ID, Object dataset) {
    population[ID].compute(dataset);
    return(population[ID].values[population[ID].getLength()-1].values[0]);
  }
  public void debug() {
    System.out.println("DEBUG");
  }

  public float[] Outputs(int ID, Object dataset) {
    System.out.println("everyone can lanch this method except processing because his compiler is very very stupid");
    population[ID].compute(dataset);
    return(population[ID].values[population[ID].getLength()].values[0]);
  }
}
