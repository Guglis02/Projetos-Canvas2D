Trabalho 2 – Transformada Discreta de Cosseno
CG - 2023/1
Gustavo Machado de Freitas

O que foi implementado:
● Leitura das amostras do arquivo de entrada no formato especificado.
● Aplicação da DCT sobre as amostras de entrada.
● Usar vetor de quantização 1D ( e plotar o resultado ). Permitir que o usuário possa
ajustar os valores.
● Aplicação da transformada inversa (IDCT) para reconstrução dos dados originais.
● Calcular a diferença entre a entrada e os dados reconstruídos.
● Plotagem dos gráficos das quatro diferentes amostragens. O tamanho do gráfico
deve se adaptar horizontalmente à quantidade de amostras lidas.
● Salvar o resultado reconstruído em um arquivo “output.dct”, no mesmo formato do
arquivo original.
● Exibir as N funções bases usadas na DCT.

Extras:
● (+1) Implementação de uma interface interativa.
● (+1) Gerador de inputs.

Interface:
● Slider Quantitização 
	Possui um range de 1 a 200.
	Determina o coeficiente de quantitização que será aplicado na DCT.
	Mudar seu valor atualiza em tempo real os gráficos.
● Slider Amostras
	Possui um range de 32 a 512, conforme especificado no enunciado.
	Determina quantas amostras serão geradas.
● Slider Frequência
  	Possui um range de 1 a 50 
	Determina a frequência para os geradores de input baseados em funções periódicas.
● Slider Amplitude
	Possui um range de 0 a 127.
	Determina a amplitude para os geradores de input baseados em funções periódicas.
● Gerar In Random
● Gerar In Seno
● Gerar In Step
● Gerar In SawTooth
	Esses botões geram inputs de acordo com os parâmetros especificados nos sliders.
● Carregar In input.dct
	Faz o load do arquivo input.dct, que contém um conjunto de valores.
● Salvar In input.dct
	Salva o input atual no arquivo input.dct.
● Salvar Out output.dct
	Salva o valor restaurado (pós IDCT) no arquivo output.dct	
  