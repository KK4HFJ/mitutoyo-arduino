# Contributing Guide

Thank you for considering contributing to the Mitutoyo 52-bit Digimatic → Arduino project!

We welcome bug reports, feature requests, documentation improvements, and code contributions.

---

## How to Contribute

1. **Fork the repository** on GitHub.
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/<your-username>/mitutoyo-arduino.git
   cd mitutoyo-arduino
   ```
3. **Create a new branch** for your changes:
   ```bash
   git checkout -b feature/my-improvement
   ```
4. **Make your changes** (code, docs, examples).
5. **Commit your changes** with a clear message:
   ```bash
   git commit -m "Add: support for ESP32 capture"
   ```
6. **Push your branch** to your fork:
   ```bash
   git push origin feature/my-improvement
   ```
7. **Submit a Pull Request** from your fork to the `main` branch of this repo.

---

## Coding Guidelines

- Use **Arduino-style C++** with clear comments.
- Keep functions small and focused.
- Always provide **serial debug prints** for new features.
- Ensure code compiles without warnings on Arduino UNO (baseline platform).

---

## Documentation Guidelines

- Add diagrams to `docs/diagrams/` in PNG format.
- Add longer guides in PDF or Markdown under `docs/`.
- Update `README.md`, `CHANGELOG.md`, and `ROADMAP.md` if relevant.

---

## Issues

- Use the **GitHub Issues tab** to report bugs or suggest features.
- When reporting bugs, include:
  - Arduino board used
  - Caliper/micrometer model
  - Wiring/setup details
  - Output logs from Serial Monitor

---

## Community Standards

- Be respectful and collaborative.
- Attribute credit where due (e.g., citing Instructables/Forum sources).
- PRs should remain on-topic and scoped to one improvement.

---

Happy hacking! 🎉
