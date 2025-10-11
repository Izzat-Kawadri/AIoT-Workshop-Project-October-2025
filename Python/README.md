# 🤖 (Flask + LLM)

This is a simple **AI-powered Flask web app** that uses a **Groq language model (LLama 3.1)** to decide whether to **turn a fan ON or OFF** based on the input temperature.

The AI makes its decision **in Arabic** and explains the reasoning behind it.

---

## 🧩 Features
- Built with **Flask** for the API backend  
- Uses **LangChain** and **ChatGroq** for AI reasoning  
- Takes temperature as input and returns a **decision in Arabic**  
- Returns a clear JSON response with emojis and timestamp  

---

## ⚙️ How It Works
1. User sends a **GET** request with a `temperature` value.  
2. The Flask app sends the prompt (in Arabic) to **Groq LLM**.  
3. The LLM responds with a decision and explanation:  
   - Temperature > 30 → "تشغيل المروحة" (Turn ON the fan)  
   - Temperature ≤ 30 → "إيقاف المروحة" (Turn OFF the fan)  
4. The result is returned as a JSON response.

---

## 🧠 Example Request

```
GET http://localhost:5000/fan?temperature=35
```

✅ Example Response
```
{
  "status": "✅ SUCCESS",
  "timestamp": "2025-10-11 12:00:00",
  "data": {
    "temperature": "35.0°م",
    "decision": "🟢 تشغيل المروحة - لأن درجة الحرارة مرتفعة"
  },
  "note": "تم اتخاذ القرار بناءً على درجة الحرارة المدخلة."
}
```

## 🔧 Setup & Run

1. Clone the repository

2. Install dependencies
```
pip install flask langchain-groq
```
3. Add your Groq API key

   Open the file and replace:

```
api_key="___"  # use your api key
```
   with your actual `API key`.

4. Run the server

```
python app.py
```
The API will run at:
```
http://0.0.0.0:5000/fan
```
---
