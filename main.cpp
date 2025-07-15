#include <iostream>
#include <random>
#include <vector>

struct Order {
    double price;
    int quantity;
    bool isBuy;
};

double brownianMotion(double currentPrice, double mu, double sigma, double dt, std::mt19937& gen) {
    std::normal_distribution<> d(mu * dt, sigma * sqrt(dt));
    return currentPrice + d(gen);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    double midPrice = 100.0;
    double mu = 0.0;
    double sigma = 1.0;
    double dt = 1.0 / 252;

    double lambda = 5.0;      // avg number of orders per dt (Poisson rate)
    std::poisson_distribution<> poisson(lambda);
    std::uniform_real_distribution<> priceOffset(-0.5, 0.5);
    std::uniform_int_distribution<> qtyDist(1, 10);
    std::bernoulli_distribution sideDist(0.5);

    std::vector<Order> orderBook;

    for (int t = 0; t < 1000; ++t) {
        midPrice = brownianMotion(midPrice, mu, sigma, dt, gen);

        int numOrders = poisson(gen);

        for (int i = 0; i < numOrders; ++i) {
            Order o;
            o.isBuy = sideDist(gen);
            o.price = midPrice + priceOffset(gen);
            o.quantity = qtyDist(gen);

            orderBook.push_back(o);
        }

        if (t % 100 == 0) {
            std::cout << "t=" << t << " midPrice=" << midPrice
                      << " orders=" << orderBook.size() << std::endl;
        }
    }

    return 0;
}