# 📊 Análise de Dados da COVID-19 com Heap em C

Este projeto foi desenvolvido como parte da disciplina **Estrutura de Dados I** do curso de Ciência da Computação da **Universidade Federal de Mato Grosso – Campus Araguaia**.

O objetivo é aplicar os conceitos de **listas encadeadas** e **Heap Máximo**, realizando uma análise dos dados da COVID-19 a partir do dataset disponibilizado pelo Kaggle.

## 🔗 Dataset utilizado
[Corona Virus Report - Kaggle](https://www.kaggle.com/datasets/imdevskp/corona-virus-report?select=country_wise_latest.csv)  
Arquivo principal: `country_wise_latest.csv`

---

## 🛠️ Funcionalidades

1. **Carregamento dos Dados:**  
   Os dados do arquivo CSV são carregados em uma lista encadeada.

2. **Escolha da métrica:**  
   O usuário pode escolher entre:
   - Número de casos confirmados
   - Número de mortes
   - Número de recuperados

3. **Visualização personalizada:**  
   Após a escolha, o usuário define quantos países deseja visualizar.

4. **Uso do Heap Máximo:**  
   Os dados são inseridos em um heap com base na métrica escolhida e exibidos em ordem decrescente de valor.

5. **Menu Interativo:**  
   Permite ao usuário retornar ao início para realizar novas buscas.

---

## 📌 Informações exibidas
- País/Região
- Valor da métrica selecionada
- Total de casos, mortes e recuperados
- Mortes por 100 habitantes
- Recuperados por 100 habitantes

---

## 📄 Relatório
Um relatório do desenvolvimento foi incluído como parte da entrega do trabalho, detalhando decisões de estrutura de dados, modularização e análise de desempenho.

---

## 🚀 Como executar

1. Clone o repositório
2. Compile o código com `gcc`
3. Execute o binário
4. Siga as instruções no terminal

---

# 📊 COVID-19 Data Analysis with Heap in C

This project was developed as part of the **Data Structures I** course in the Computer Science program at the **Federal University of Mato Grosso – Araguaia Campus**.

The goal is to apply **linked lists** and **Max Heap** concepts to analyze COVID-19 data using a dataset provided by Kaggle.

## 🔗 Dataset used
[Corona Virus Report - Kaggle](https://www.kaggle.com/datasets/imdevskp/corona-virus-report?select=country_wise_latest.csv)  
Main file: `country_wise_latest.csv`

---

## 🛠️ Features

1. **Data Loading:**  
   CSV data is loaded into a linked list.

2. **Metric Selection:**  
   The user can choose to view:
   - Number of confirmed cases
   - Number of deaths
   - Number of recovered

3. **Custom View:**  
   User inputs how many top countries they want to display.

4. **Max Heap Usage:**  
   Data is inserted into a max heap based on the selected metric and displayed in descending order.

5. **Interactive Menu:**  
   Allows users to perform multiple searches.

---

## 📌 Displayed Information
- Country/Region
- Selected metric value
- Total cases, deaths, and recovered
- Deaths per 100 inhabitants
- Recoveries per 100 inhabitants

---

## 📄 Report
A development report is included, describing data structure decisions, modularization, and performance analysis.

---

## 🚀 How to Run

1. Clone this repository
2. Compile using `gcc`
3. Run the binary
4. Follow the terminal instructions
