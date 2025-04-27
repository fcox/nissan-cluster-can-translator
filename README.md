# Nissan Cluster CAN Translator

## Project Description

The **Nissan Cluster CAN Translator** project is an application designed to translate and process CAN (Controller Area
Network) bus messages used in Nissan vehicle instrument clusters. This was born as way to retrofit a **Nissan Sentra B17
** cluster
into a **Nissan Tiida (Versa in other countries) C11** enabling features like On-Board computer and Engine temperature
gauge

### Main Features:

- **CAN Decoding:** Translates and decodes CAN messages specific to Nissan vehicle clusters.
- **Compatibility:** Built using C++11, ensuring modularity, performance, and a robust technological foundation.

---

## Project Structure

The project follows the general structure outlined below:

- **Source Code:** Contains the core logic for decoding, handling the CAN bus, and auxiliary tools.

---

## Prerequisites

### Required Hardware:

1. **Arduino board:** Tested with an arduino nano clone (Atmega328, old bootloader).
2. **[MCP2515 SPI board](https://www.amazon.de/dp/B01IV3ZSKO/):** Feel free to use any MCP2515 board.

### Required Tools:

1. **PlatformIO Core:** Used to compile, test, and manage hardware-related packages for embedded development.
    - Documentation: [PlatformIO](https://docs.platformio.org/)
2. **C++11 Programming Language:** Ensure you have a compiler that supports this standard.
3. **CAN Interface:** You’ll need access to compatible hardware to communicate with a vehicle’s CAN bus.

---

## Installation

1. Clone the project repository:
   ```bash
   git clone <REPOSITORY_URL>
   cd nissan-cluster-can-translator
   ```

2. Set up **PlatformIO Core**:
   ```bash
   pip install platformio
   ```

3. Build the code:
   ```bash
   platformio run
   ```

4. Run tests:
   ```bash
   platformio test
   ```

---

## Usage

### Running the Application:

1. Connect external hardware to the CAN interface.
2. Launch the developed application.
3. Monitor CAN messages from the vehicle's cluster.

### Testing Process:

Execute unit tests to ensure proper functionality. All test files are located in the `/test` directory.

- Learn more about PlatformIO
  testing: [Unit Testing PlatformIO](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)

---

## Contribution

Any contributions to the project are welcome. Please:

1. **Fork** this repository.
2. Create a branch for your feature or fix (`git checkout -b feature/new-feature`).
3. Submit a **pull request**.

---

## License

Please check the `LICENSE` file in the repository for details on the software's rights and restrictions.

---

## Additional Resources

- Automated testing
  documentation: [PlatformIO Unit Testing](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)
- C++11 Specification: Refer to technical manuals or official documentation for specific language-related inquiries.

---

If you have any questions or require technical support, feel free to open an **issue**. Thank you for using Nissan
Cluster CAN Translator! 🚗