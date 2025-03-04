from flask import Flask, request, jsonify
import mysql.connector

app = Flask(__name__)

def get_db_connection():
    connection = mysql.connector.connect(
        host='localhost',
        user='root',
        password='root',
        database='data' 
    )
    return connection

@app.route('/upload_distance', methods=['GET'])
def upload_distance():
    try:
        distance = request.args.get('distance')
        
        if not distance:
            return jsonify({"error": "Distance not provided"}), 400
        
        distance = int(distance)

        conn = get_db_connection()
        cursor = conn.cursor()

        cursor.execute('''
            INSERT INTO DistanceData (distance) VALUES (%s)
        ''', (distance,))

        conn.commit()
        cursor.close()
        conn.close()

        return jsonify({"message": f"Distance {distance} successfully uploaded"}), 200

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
