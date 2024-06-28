import numpy as np
from matplotlib import pyplot as plt
import sqlite3
from tableStructure import TxRxStats

tableName = 'pktTxRx'

# Connect to the database
connection = sqlite3.connect('../outputs/default-simulator-outputs.db')
cursor = connection.cursor()

# Query the database
cursor.execute("SELECT * FROM " + tableName)
rows = cursor.fetchall()

# Convert each row into an instance of TxRxStats and add it to a list
data_TxRxStats = [TxRxStats(*row) for row in rows]

x = np.arange(0, 3 * np.pi, 0.1)
y_sin = np.sin(x)
y_cos = np.cos(x)

# Set up a subplot grid that has height 2 and width 1,
# and set the first such subplot as active.
plt.subplot(2, 1, 1)

# Make the first plot
plt.plot(x, y_sin)
plt.title('Sine')

# Set the second subplot as active, and make the second plot.
plt.subplot(2, 1, 2)
plt.plot(x, y_cos)
plt.title('Cosine')

# Show the figure.
plt.show()



# Close the connection
connection.close()