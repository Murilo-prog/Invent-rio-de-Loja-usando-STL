#include <iostream>
#include <vector>   
#include <list>     
#include <set>      
#include <map>      
#include <string>

using namespace std;

class Produto {
public:
    string nome;
    string categoria;
    double preco;
    int estoque;

    Produto(string n, string c, double p, int e)
        : nome(n), categoria(c), preco(p), estoque(e) {}
};

class Inventario {
private:
    vector<Produto> produtos;                     // Lista de produtos
    set<string> categorias;                       // Categorias únicas
    map<string, int> contagemPorCategoria;        // Quantidade por categoria
    map<string, double> valorPorCategoria;        // Valor total em estoque por categoria
    list<Produto> historicoVendas;                // Histórico de produtos vendidos

public:

    // Adicionar produto
    void adicionarProduto(const Produto& p) {
        produtos.push_back(p);

        categorias.insert(p.categoria); // set evita duplicatas automaticamente

        contagemPorCategoria[p.categoria] += 1;
        valorPorCategoria[p.categoria] += p.preco * p.estoque;
    }

    // Buscar produto por nome
    Produto* buscarProduto(const string& nome) {
        for (auto& p : produtos) {   // range-based for
            if (p.nome == nome) {
                return &p;
            }
        }
        return nullptr;
    }

    // Listar produtos de uma categoria
    void listarCategoria(const string& categoria) const {
        cout << "Produtos da categoria '" << categoria << "':\n";

        for (const auto& p : produtos) {
            if (p.categoria == categoria) {
                cout << "- " << p.nome << " | R$" << p.preco 
                     << " | Estoque: " << p.estoque << "\n";
            }
        }
    }

    // Calcular valor total do inventário
    double valorTotalInventario() const {
        double total = 0;

        for (const auto& p : produtos) {
            total += p.preco * p.estoque;
        }

        return total;
    }

    // Registrar venda (remove do estoque e salva histórico)
    bool registrarVenda(const string& nome, int quantidade) {
        Produto* p = buscarProduto(nome);

        if (!p) {
            cout << "Produto não encontrado!\n";
            return false;
        }

        if (p->estoque < quantidade) {
            cout << "Estoque insuficiente!\n";
            return false;
        }

        // Atualiza estoque
        p->estoque -= quantidade;

        // Corrige valor total da categoria
        valorPorCategoria[p->categoria] -= p->preco * quantidade;

        // Registra no histórico
        historicoVendas.push_back(Produto(p->nome, p->categoria, p->preco, quantidade));

        cout << "Venda registrada!\n";
        return true;
    }

    // Mostrar histórico de vendas
    void mostrarHistorico() const {
        cout << "\n=== HISTÓRICO DE VENDAS ===\n";
        for (const auto& v : historicoVendas) {
            cout << v.nome << " | Categoria: " << v.categoria
                 << " | Qtd: " << v.estoque 
                 << " | Preço: R$" << v.preco << "\n";
        }
    }

    // Mostrar resumo por categoria
    void mostrarResumoCategorias() const {
        cout << "\n=== RESUMO POR CATEGORIA ===\n";

        for (const auto& cat : categorias) {
            cout << "Categoria: " << cat << "\n";

            auto itCount = contagemPorCategoria.find(cat);
            auto itValue = valorPorCategoria.find(cat);

            int count = (itCount != contagemPorCategoria.end()) ? itCount->second : 0;
            double value = (itValue != valorPorCategoria.end()) ? itValue->second : 0.0;

            cout << "  Produtos: " << count << "\n";
            cout << "  Valor total: R$" << value << "\n";
        }
    }
};

int main() {
    Inventario loja;

    loja.adicionarProduto(Produto("TV Samsung", "Eletrônicos", 2500.0, 5));
    loja.adicionarProduto(Produto("Notebook Dell", "Eletrônicos", 4200.0, 3));
    loja.adicionarProduto(Produto("Camisa Polo", "Vestuário", 120.0, 20));

    cout << "\nValor total do inventário: R$" 
         << loja.valorTotalInventario() << "\n";

    loja.listarCategoria("Eletrônicos");

    loja.registrarVenda("Camisa Polo", 3);
    loja.registrarVenda("TV Samsung", 1);

    loja.mostrarHistorico();
    loja.mostrarResumoCategorias();

    return 0;
}
