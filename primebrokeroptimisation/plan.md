<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>PB Trade Assignment Optimizer Design</title>
  <style>
    body { font-family: Arial, sans-serif; line-height: 1.6; padding: 20px; max-width: 960px; margin: auto; }
    h1, h2, h3 { color: #fff; } /* Titles to white */
    pre { background: none; padding: 10px; border-left: 4px solid #ccc; overflow-x: auto; } /* Remove background */
    code { background: none; padding: 2px 4px; font-family: monospace; } /* Remove background */
    table { width: 100%; border-collapse: collapse; margin: 20px 0; }
    th, td { border: 1px solid #ccc; padding: 8px; text-align: left; }
    th { background: #f0f0f0; }
  </style>
</head>
<body>

<h1>📊 Trade Assignment Optimizer for PB Accounts</h1>

<h2>🔍 Introduction</h2>
<p>
  As a firm, one of the challenges we face is how we finance our positions. Hedge funds traditionally use Prime Brokerage (PB)
  services to custody assets and finance short positions in cash and securities. Each PB account manages counterparty risk by
  enforcing constraints on account metrics and pricing financing differently. When executing trades, we must select the most optimal
  PB account to assign each trade.
</p>

<h2>🎯 Objectives</h2>
<ul>
  <li><strong>Primary:</strong> Assign each trade to a PB account such that all <strong>hard constraints</strong> are satisfied under <strong>stochastic changes</strong> to security values.</li>
  <li><strong>Secondary:</strong> Optimize <strong>Objective Priorities (OPs)</strong> like:
    <ul>
      <li>Minimizing aggregate financing cost</li>
      <li>Maximizing metric headroom (constraint buffer)</li>
      <li>Minimizing portfolio risk</li>
    </ul>
  </li>
</ul>

<h2>📥 Inputs</h2>
<ul>
  <li><strong>Trades (0 ≤ n ≤ 200):</strong>
    <ul>
      <li>Side: [buy, sell]</li>
      <li>Security name</li>
      <li>Security type: [corporate, government]</li>
      <li>Quantity</li>
      <li>Price</li>
      <li>Accrued interest</li>
    </ul>
  </li>
  <li><strong>Accounts (2 ≤ k ≤ 5):</strong>
    <ul>
      <li>Current cash and positions</li>
      <li>Metric definitions: <code>M_a,t = Σ ω_a,t,s ⋅ V_a,t,s</code></li>
      <li>Metric constraints: (≤, ≥)</li>
      <li>Optimization priorities and weights</li>
    </ul>
  </li>
  <li><strong>Stochastic Parameters:</strong>
    <ul>
      <li>Security price volatilities or simulated scenarios</li>
    </ul>
  </li>
</ul>

<h2>📤 Outputs</h2>
<ul>
  <li>Trade → Account assignment</li>
  <li>Post-assignment metric values for all accounts</li>
  <li>Constraint compliance report</li>
</ul>

<h2>🧠 Optimization Model</h2>

<h3>🧩 Decision Variables</h3>
<pre><code>x[t,a] ∈ {0, 1} : Trade t is assigned to Account a</code></pre>

<h3>✅ Constraints</h3>
<ol>
  <li><strong>Unique assignment:</strong><br>
    <code>∑ x[t,a] = 1 for all t</code>
  </li>
  <li><strong>Cash and positions post-trade:</strong><br>
    <code>cash_new = cash - ∑ (x[t,a] × trade_cost_t)</code>
  </li>
  <li><strong>Metric constraints (under all scenarios):</strong><br>
    <code>M_a,t(scenario) + ΔM_from_trades ≤ or ≥ C</code>
  </li>
</ol>

<h3>🧮 Objective Function</h3>
<p>Minimize:</p>
<pre><code>∑ x[t,a] × cost[t,a] × weight_priority</code></pre>

<h2>🎲 Stochastic Modeling</h2>
<ul>
  <li>Use scenario-based stress testing</li>
  <li>Simulate N possible bond price shifts using distributional assumptions</li>
  <li>Validate that hard constraints hold under all scenarios</li>
</ul>

<h2>🏗️ Architecture & Patterns</h2>

<h3>🔧 Module Layout</h3>
<pre>
PortfolioOptimizer
├── TradeAssigner (MIP model builder)
├── MetricEngine (calculates metric impacts)
├── ScenarioGenerator (price simulations)
├── Validator (verifies post-trade constraints)
└── ReportBuilder (outputs metrics, assignments)
</pre>

<h3>🧱 Design Patterns Used</h3>
<table>
  <thead>
    <tr>
      <th>Pattern</th>
      <th>Purpose</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Strategy</td>
      <td>Plug in different solver strategies (MIP, fallback)</td>
    </tr>
    <tr>
      <td>Builder</td>
      <td>Compose MIP model incrementally</td>
    </tr>
    <tr>
      <td>Factory</td>
      <td>Generate metric calculators by type</td>
    </tr>
    <tr>
      <td>Separation of Concerns</td>
      <td>Decouples metric logic, optimization, reporting</td>
    </tr>
  </tbody>
</table>

<h2>🧱 Code Flow (Pseudocode)</h2>
<pre><code>optimizer = PortfolioOptimizer(trades, accounts, metric_defs, constraints, optimization_weights)

scenarios = optimizer.generate_scenarios()

model = optimizer.build_mip_model(scenarios)

solution = optimizer.solve_model(model)

assignments = optimizer.extract_solution(solution)

report = optimizer.build_report(assignments)
</code></pre>

<h2>🧩 OR-Tools Variable Definition Example</h2>
<pre><code>
# x[t, a] = 1 if trade t is assigned to account a
x = {}
for t in trades:
    for a in accounts:
        x[t.id, a.id] = solver.BoolVar(f'x_{t.id}_{a.id}')
</code></pre>

<h2>✅ Maintainability Considerations</h2>
<ul>
  <li>Config-driven constraint definitions (no hardcoded logic)</li>
  <li>Audit trail for trade → account mapping</li>
  <li>Scenario validation built in</li>
  <li>Fail-safe greedy fallback if MIP fails</li>
  <li>Unit and scenario tests for each component</li>
  <li>Clear documentation: input schema, outputs, metric formulas</li>
</ul>

<h2>📈 Summary Table</h2>
<table>
  <thead>
    <tr>
      <th>Component</th>
      <th>Choice/Method</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Optimization Engine</td>
      <td>Google OR-Tools (SCIP MIP solver)</td>
    </tr>
    <tr>
      <td>Decision Variables</td>
      <td>x[t, a] ∈ {0, 1}</td>
    </tr>
    <tr>
      <td>Stochastic Modeling</td>
      <td>Scenario-based with worst-case constraint evaluation</td>
    </tr>
    <tr>
      <td>Constraints</td>
      <td>Account-specific, hard, enforced under all scenarios</td>
    </tr>
    <tr>
      <td>Objective</td>
      <td>Minimize weighted OPs (e.g., financing cost)</td>
    </tr>
    <tr>
      <td>Design Pattern</td>
      <td>Strategy, Builder, Factory, SoC</td>
    </tr>
  </tbody>
</table>

</body>
</html>
