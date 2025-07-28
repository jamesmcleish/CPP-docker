<section>
  <h2>Executive Summary</h2>
  <p>
    This plan outlines the implementation of an execution optimization framework for a hedge fund operating across multiple prime brokers. The primary objective is to develop and deploy a cost reduction algorithm that intelligently allocates trades to prime brokers, minimizing overall execution costs while respecting key constraints such as liquidity, risk limits, and regulatory requirements. By integrating advanced optimization techniques, the system will enhance decision-making in trade assignment, balancing cost efficiency with operational feasibility. Successful implementation will lead to significant reductions in trading expenses, improved execution quality, and a stronger competitive position for the hedge fund.
  </p>
</section>

<section>
  <h2>Algorithm Overview: Validation and Fulfillment Stages</h2>
  <p>
    The diagram below provides a high-level overview of the execution optimization algorithm. It illustrates the two key stages of the process: the Validation Phase, where constraints and business rules are applied to ensure feasibility, and the Fulfillment Phase, where optimization priorities guide the selection of the best trade assignments. This structured approach ensures that all trade allocations comply with necessary restrictions while achieving cost-efficient execution across multiple prime brokers.
  </p>


```mermaid
flowchart TD
    %% Node Definitions
    A(["<b>Trade</b>"]) --> B
    B(["<b>Validation Phase</b><br><i>Rule-based Checks</i><br><font color='#000000' size='2'>Check accounts against metrics with hard constraints for viability and eliminate prime brokerages from selection.</font>"]) --> C
    C(["<b>Fulfillment Phase</b><br><i>Inventory & Logistics</i><br><font color='#000000' size='2'>Allocate stock and schedule delivery<br>based on availability and priority</font>"]) --> D
    D(["<b>Execution</b>"])
    

    %% Inputs
    subgraph Inputs
        G(["<b>Accounts information</b>"])-->B
        E["<b>Metrics with constraints</b>"] --> B
        F["<b>Metrics with optimisation priorities</b>"] --> C
    end

    %% Styles
    style A fill:#e3f2fd,color:#0d47a1,stroke:#0d47a1,stroke-width:1px
    style B fill:#bbdefb,color:#0d47a1,stroke:#0d47a1,stroke-width:1px
    style C fill:#90caf9,color:#0d47a1,stroke:#0d47a1,stroke-width:1px
    style D fill:#64b5f6,color:#0d47a1,stroke:#0d47a1,stroke-width:1px
    style E fill:#f0f4c3,color:#827717,stroke:#827717,stroke-width:1px
    style F fill:#fce4ec,color:#880e4f,stroke:#880e4f,stroke-width:1px

```
</section>

```mermaid
flowchart TD
    %% Inputs
    A1["Accounts<br>• trades<br>• cash<br>• metrics: 1 &lt;= k &lt;= 20"]
    A2["Portfolio<br>• accounts"]
    A3["Trades<br>• side: buy/sell<br>• security name<br>• type: gov or corp<br>• quantity<br>• price<br>• accrued interest"]

    %% Derived metrics
    M["Metric Calculations<br>• metric value<br>• constraint (optional)<br>• constraint type<br>• optimization priority"]

    %% Optimization context
    OP["Optimization Priorities<br>• description<br>• weight"]

    %% Engine
    E["Optimization Engine<br>• Apply hard constraints<br>• Score priorities<br>• Optimize assignment"]

    %% Output
    R["Trade to Account Assignment"]

    %% Arrows and Flow
    A1 --> M
    A2 --> A1
    A3 --> M
    M --> E
    OP --> E
    E --> R

    %% Styling
    style A1 fill:#e0f7fa,stroke:#00796b,color:#004d40
    style A2 fill:#f1f8e9,stroke:#689f38,color:#33691e
    style A3 fill:#fce4ec,stroke:#d81b60,color:#880e4f
    style M fill:#fff3e0,stroke:#fb8c00,color:#e65100
    style OP fill:#ede7f6,stroke:#5e35b1,color:#311b92
    style E fill:#e3f2fd,stroke:#1976d2,color:#0d47a1
    style R fill:#dcedc8,stroke:#558b2f,color:#33691e
```

