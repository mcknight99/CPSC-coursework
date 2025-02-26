from qiskit import QuantumCircuit
# from qiskit_ibm_provider import IBMProvider

qc = QuantumCircuit(2)
qc.h(0)
qc.cx(0, 1)
qc.measure_all()
print(qc)

# IBMProvider.save_account('c7c609c0e3f3332a6dcb4f6b8aeca416f8dcaa15b544fd0141cd8cb5d03165ba948339a6cf342d8aee023419390a772224615bb1b97532260403972ed7eb138f')
# IBMProvider.load_account()

# Alternatively, if you do not want to save your credentials to disk and only intend to use them during the current session, you can use:

# provider = IBMQ.enable_account('MY_API_TOKEN')
# backend = provider.get_backend('ibmq_qasm_simulator')