#include<bits/stdc++.h>
#define ll long long
#define vi vector<int>
#define vii vector<vector<int>>
#define optimize ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)

const int match = 3;
const int mismactch = -1;
const int gap = -2;

using namespace std;
void formataMatriz(int maxWidth,int width,  ofstream& arquivo){
    int numeroEspacos = maxWidth - width;
    for(int i = 0; i < numeroEspacos +2;i++) arquivo << " ";
}

void constroiMatrizScore(vector<vector<int>>& MatrizScores, string dnaVertical, string dnaHorizontal, int& maxWidth){
    int tamanhoDnaVertical = dnaVertical.size();
    int tamanhoDnaHorizontal = dnaHorizontal.size();

    //Caso base 1: Quando as duas strings são vazias, o score é zero. 
    MatrizScores[0][0] = 0;


    //Caso base 2: Caso  uma das strings seja vazia e a outra não, com certeza o alinhamento será composto apenas de
    //gaps, pois para cada caractere da string não vazia, não há nenhum caractere na outra string para dar match
    //ou mismatch.
    for(int linha = 1; linha <= tamanhoDnaVertical; linha++){
        MatrizScores[linha][0] = MatrizScores[linha - 1][0] + gap;
    }

    for(int coluna = 1; coluna <= tamanhoDnaHorizontal; coluna++){
        MatrizScores[0][coluna] = MatrizScores[0][coluna - 1] + gap;
    }

    maxWidth = 0;
    for(int linha = 1; linha <= tamanhoDnaVertical; linha++){
        for(int coluna = 1; coluna <= tamanhoDnaHorizontal; coluna++){
            int gapVertical = MatrizScores[linha-1][coluna] + gap;
            int gapHorizontal = MatrizScores[linha][coluna-1] + gap;
            int tentativaDeMatch = MatrizScores[linha-1][coluna-1];

            if(linha >=1 && coluna >=1 ){
                if(dnaHorizontal[coluna-1] == dnaVertical[linha-1]) tentativaDeMatch += match;
                else tentativaDeMatch += mismactch;
            }
            else{
                tentativaDeMatch = -1e8;
            }

            MatrizScores[linha][coluna] = max(tentativaDeMatch, max(gapHorizontal, gapVertical));       
            int width = to_string(MatrizScores[linha][coluna]).size();
            if(width > maxWidth) maxWidth = width;
        }
    }


}

void constroiAlinhamentoGlobal(vector<vector<int>>& matrizScores, string dnaVertical, string dnaHorizontal,vector<vector<char>>& alinhamento){
    int tamanhoDnaVertical = dnaVertical.size();
    int tamanhoDnaHorizontal =dnaHorizontal.size();
    int tamanhoAlinhamento = alinhamento.size();


    int indiceAlinhamento = tamanhoAlinhamento - 1;
    int linha = tamanhoDnaVertical;
    int coluna = tamanhoDnaHorizontal;

    while(!(linha == 0 && coluna == 0)){
        if(linha == 0){
            alinhamento[indiceAlinhamento][0] = '-';
            alinhamento[indiceAlinhamento][1] = dnaHorizontal[coluna-1];
            coluna--;
        }
        else if(coluna == 0){
            alinhamento[indiceAlinhamento][0] = dnaVertical[linha-1];
            alinhamento[indiceAlinhamento][1] = '-';
            linha--;
        }
        else{
            int gapVertical = matrizScores[linha-1][coluna] + gap;
            int gapHorizontal = matrizScores[linha][coluna-1] + gap;
            int tentativaDeMatch = matrizScores[linha-1][coluna-1];
            if(linha >=1 && coluna >=1 ){
                if(dnaHorizontal[coluna-1] == dnaVertical[linha-1]) tentativaDeMatch += match;
                else tentativaDeMatch += mismactch;
            }
            else{
                tentativaDeMatch = -1e8;
            }
            int maior = max(tentativaDeMatch, max(gapHorizontal, gapVertical));
            if(maior == tentativaDeMatch){
                
                alinhamento[indiceAlinhamento][0] = dnaVertical[linha-1];
                alinhamento[indiceAlinhamento][1] = dnaHorizontal[coluna-1];
                linha--;
                coluna--;
            }
            else if(maior == gapVertical){
                
                alinhamento[indiceAlinhamento][0] = dnaVertical[linha-1];
                alinhamento[indiceAlinhamento][1] = '-';
                linha--;

            }
             else if(maior == gapHorizontal){
                
                alinhamento[indiceAlinhamento][0] = '-';
                alinhamento[indiceAlinhamento][1] = dnaHorizontal[coluna-1];
                coluna--;

            }

        }

        indiceAlinhamento--;

    }



}

void imprimeSaida(vector<vector<int>>& matrizScores, vector<vector<char>>& alinhamento,string dnaVertical, string dnaHorizontal, int maxWidth){
    int score = matrizScores[matrizScores.size()-1][matrizScores[0].size()-1];
    ofstream arquivo("output.txt");
    //imprime matriz de Scores
    arquivo << "Dna vertical: " << dnaVertical << endl;
    arquivo << "Dna horizontal: " << dnaHorizontal << endl;
    arquivo << "Matriz de scores: " << endl;
    int indiceDnaVertical = dnaVertical.size()-1;
    for(int linha = matrizScores.size()-1; linha >= 0; linha--){
        if(linha != 0){
            arquivo << dnaVertical[indiceDnaVertical--];
        }
        else{
            arquivo << 'U';
        }

        for(int score : matrizScores[linha]){
            formataMatriz(maxWidth, to_string(score).size(),arquivo);
            arquivo << score;
        }

        arquivo << endl;

    }

    arquivo << 'X';
    formataMatriz(maxWidth, to_string(1).size(),arquivo);
    arquivo << 'U';

    for(char base : dnaHorizontal){
        formataMatriz(maxWidth, to_string(1).size(), arquivo);
        arquivo << base;
    }
    arquivo << endl;


    arquivo << "Score = " << score << " | Match = " << match << " | Mismatch = " << mismactch <<" | Gap = " << gap;
    arquivo << endl;
    arquivo << "Alinhamento: " << endl;
    arquivo << "dna1: ";
    for(auto x : alinhamento){
        if(x[0] == '0')continue;
        arquivo <<" " << x[0];
    }
    arquivo << endl;
    arquivo << "dna2: ";
    for(auto y : alinhamento){
        if(y[1] == '0') continue;
        arquivo <<" " << y[1];
    }
    arquivo << endl;




}

void recebeEntrada(string& s, string& t){
    ifstream arquivo("input.txt");
    s = "";
    t = "";
    if(arquivo.is_open()){
        getline(arquivo,s);
        getline(arquivo,t);
        cout << "input aberto com sucesso" << endl;
       

    }
    else{
        cout << "erro ao abrir o arquivo de input" << endl;
    }


    
}





int main(){


    optimize;


    string dnaVertical, dnaHorizontal;
    recebeEntrada(dnaVertical,dnaHorizontal);
    

    int tamanhoDnaVertical = dnaVertical.size();
    int tamanhoDnaHorizontal = dnaHorizontal.size();

    vector<vector<int>>matrizScores(tamanhoDnaVertical+1, vector<int>(tamanhoDnaHorizontal+1));


    int tamanhoAlinhamento =tamanhoDnaHorizontal + tamanhoDnaVertical;
    vector<vector<char>> alinhamento(tamanhoAlinhamento, vector<char>(2,'0'));

    int maxWidth = 0;
    cout << "construindo matriz de Score" << endl;
    constroiMatrizScore(matrizScores,dnaVertical,dnaHorizontal,maxWidth);
    cout << "Fazendo o backtracking para descobrir o alinhamento" << endl;
    constroiAlinhamentoGlobal(matrizScores,dnaVertical,dnaHorizontal,alinhamento);
    cout << "Gerando resultados" << endl;
    imprimeSaida(matrizScores,alinhamento,dnaVertical,dnaHorizontal,maxWidth);
    cout << "End of Program" << endl;






   

}