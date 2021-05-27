from flask import Flask, request
import sqlite3, uuid, json
 
app = Flask(__name__)

@app.route('/registration', methods=['POST'])
def registration():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    login = request.form.get('login')
    password = request.form.get('password')
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ?"""
        sql_data = (login, )
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            json_data = {"type": "registration_info", "success": False, "info": "Логин зарегистрирован!"}
            conn.close()
            return json.dumps(json_data)
        else:
            sql_param = """INSERT INTO users(Login, Password)
                          VALUES (?, ?)"""
            sql_data = (login, password)
            cursor.execute(sql_param, sql_data)
            conn.commit()
            
            json_data = {"type": "registration_info", "success": True}
            conn.close()
            return json.dumps(json_data)
            
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "registration_info", "success": False, "info": "Серверная ошибка!"}
        conn.close()
        return json.dumps(json_data)

@app.route('/authentification', methods=['POST'])
def authentification():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    login = request.form.get('login')
    password = request.form.get('password')
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND  Password = ?"""
        sql_data = (login, password)
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            UserID = str(uuid.uuid4())
            sql_param = """UPDATE users SET UserID = ? WHERE Login = ?"""
            sql_data = (UserID, login)
            cursor.execute(sql_param, sql_data)
            conn.commit()
            
            json_data = {"type": "authentification_info", "success": True, "access_id": UserID}
            conn.close()
            return json.dumps(json_data)
            
        else:
            json_data = {"type": "authentification_info", "success": False, "info": "Неправильный логин или пароль!"}
            conn.close()
            return json.dumps(json_data)
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "authentification_info", "success": False, "info": "Серверная ошибка!"}
        conn.close()
        return json.dumps(json_data)
    
@app.route('/addproduct', methods=['POST'])
def addproduct():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    user = request.form.get('user')
    access_id = request.form.get('access_id')
    date = request.form.get('date')
    products = request.form.get('products')
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND UserID = ?"""
        sql_data = (user, access_id)
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            sql_param = """SELECT Products FROM foods WHERE id = ? AND DATE = ?"""
            sql_data = (data[0], date)
            cursor.execute(sql_param, sql_data)
            info = cursor.fetchone()
            
            if info is None:
                sql_param = """INSERT INTO foods(id, DATE, Products)
                              VALUES (?, ?, ?)"""
                sql_data = (data[0], date, products)
                cursor.execute(sql_param, sql_data)
                conn.commit()
            else:
                sql_param = """UPDATE foods SET Products = ? WHERE id = ? AND DATE = ?"""
                sql_data = (products, data[0], date)
                cursor.execute(sql_param, sql_data)
                conn.commit()
            json_data = {"type": "addproduct_info", "success": True}
            conn.close()
            return json.dumps(json_data)
        else:
            json_data = {"type": "addproduct_info", "success": False, "info": "Ошибка авторизации, перезайдите в приложение!"}
            conn.close()
            return json.dumps(json_data)
        
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "addproduct_info", "success": False, "info": "Серверная ошибка!"}
        conn.close()
        return json.dumps(json_data)

@app.route('/getproduct', methods=['POST'])
def getproduct():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    user = request.form.get('user')
    access_id = request.form.get('access_id')
    date = request.form.get('date')
    
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND UserID = ?"""
        sql_data = (user, access_id)
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            sql_param = """SELECT Products FROM foods WHERE id = ? AND DATE = ?"""
            sql_data = (data[0], date)
            
            cursor.execute(sql_param, sql_data)
            
            info = cursor.fetchone()
            
            if info is not None:
                json_data = {"type": "getproduct_info", "success": True, "info": info[0]}
                conn.close()
                return json.dumps(json_data)
            else:
                json_data = {"type": "getproduct_info", "success": False}
                conn.close()
                return json.dumps(json_data)
        else:
            json_data = {"type": "getproduct_info", "success": False, "info": "Ошибка авторизации, перезайдите в приложение!"}
            conn.close()
            return json.dumps(json_data)
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "getproduct_info", "success": False, "info": "Серверная ошибка! Данные могут быть не сохранены!"}
        conn.close()
        return json.dumps(json_data)

@app.route('/addfavorite', methods=['POST'])
def addfavorite():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    user = request.form.get('user')
    access_id = request.form.get('access_id')
    name = request.form.get('name')
    products = request.form.get('products')
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND UserID = ?"""
        sql_data = (user, access_id)
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            sql_param = """SELECT Products FROM favorite WHERE id = ? AND Name = ?"""
            sql_data = (data[0], name)
            cursor.execute(sql_param, sql_data)
            info = cursor.fetchone()
            
            if info is None:
                sql_param = """INSERT INTO favorite(id, Name, Products)
                              VALUES (?, ?, ?)"""
                sql_data = (data[0], name, products)
                cursor.execute(sql_param, sql_data)
                conn.commit()
            else:
                sql_param = """UPDATE favorite SET Products = ? WHERE id = ? AND Name = ?"""
                sql_data = (products, data[0], name)
                cursor.execute(sql_param, sql_data)
                conn.commit()
            json_data = {"type": "addfavorite_info", "success": True}
            conn.close()
            return json.dumps(json_data)
        else:
            json_data = {"type": "addfavorite_info", "success": False, "info": "Ошибка авторизации, перезайдите в приложение!"}
            conn.close()
            return json.dumps(json_data)
        
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "addfavorite_info", "success": False, "info": "Серверная ошибка!"}
        conn.close()
        return json.dumps(json_data)

@app.route('/getfavorite', methods=['POST'])
def getfavorite():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    user = request.form.get('user')
    access_id = request.form.get('access_id')
    
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND UserID = ?"""
        sql_data = (user, access_id)
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            sql_param = """SELECT Name, Products FROM favorite WHERE id = ?"""
            sql_data = (data[0],)
            
            cursor.execute(sql_param, sql_data)
            
            info = cursor.fetchall()

            if info is not None:
                lis = []
                for i in range(len(info)):
                    dic = {}
                    dic['name'] = info[i][0]
                    dic['info'] = info[i][1]
                    lis.append(dic)
                json_data = {"type": "getfavorite_info", "success": True, "info": lis}
                print(json_data)
                conn.close()
                return json.dumps(json_data)
            else:
                json_data = {"type": "getfavorite_info", "success": False}
                conn.close()
                return json.dumps(json_data)
        else:
            json_data = {"type": "getfavorite_info", "success": False, "info": "Ошибка авторизации, перезайдите в приложение!"}
            conn.close()
            return json.dumps(json_data)
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "getfavorite_info", "success": False, "info": "Серверная ошибка! Данные могут быть не сохранены!"}
        conn.close()
        return json.dumps(json_data)

@app.route('/delfavorite', methods=['POST'])
def delfavorite():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    
    user = request.form.get('user')
    access_id = request.form.get('access_id')
    name = request.form.get('name')
    
    
    try:
        sql_param = """SELECT id FROM users WHERE Login = ? AND UserID = ?"""
        sql_data = (user, access_id)
        
        cursor.execute(sql_param, sql_data)
        
        data = cursor.fetchone()
        
        if data is not None:
            sql_param = """DELETE FROM favorite WHERE id = ? AND Name = ?"""
            sql_data = (data[0], name)
            
            cursor.execute(sql_param, sql_data)
            conn.commit()
            
            json_data = {"type": "delfavorite_info", "success": True}
            conn.close()
            return json.dumps(json_data)
        else:
            json_data = {"type": "delfavorite_info", "success": False, "info": "Ошибка авторизации, перезайдите в приложение!"}
            conn.close()
            return json.dumps(json_data)
    except sqlite3.Error as error:
        print("Ошибка при работе с SQLite", error)
        json_data = {"type": "delfavorite_info", "success": False, "info": "Серверная ошибка! Данные могут быть не сохранены!"}
        conn.close()
        return json.dumps(json_data)

if __name__ == "__main__":
    app.run(host = "192.168.0.121", port = 5000)