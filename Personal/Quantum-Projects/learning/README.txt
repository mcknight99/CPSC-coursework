process:
pip install qiskit
pip install notebook

Install/Enable suggested extensions (python, jupyter, pylance)


To run on real quantum computers (IBM):

pip install qiskit-ibmq-provider --user

https://quantum.ibm.com/
Setup IBM Quantum Account
Generate API token


Got the following error 
ImportError: Qiskit is installed in an invalid environment that has both Qiskit >=1.0 and an earlier version. 
You should create a new virtual environment, and ensure that you do not mix dependencies between Qiskit <1.0 and >=1.0. 
Any packages that depend on 'qiskit-terra' are not compatible with Qiskit 1.0 and will need to be updated. 
Qiskit unfortunately cannot enforce this requirement during environment resolution. 
See https://qisk.it/packaging-1-0 for more detail.
https://docs.quantum.ibm.com/migration-guides/qiskit-1.0-installation#import-qiskit-raises-importerror

Solving:
pip install pipdeptree
pipdeptree --reverse --package qiskit-terra
pipdeptree --reverse --package qiskit

anything not depending on qiskit 1.0+ is what is causing the issue, might need to downgrade qiskit to -1.0 if it can't be installed with a dep of qiskit1.0+