from flask import Flask, request, jsonify
from langchain_groq import ChatGroq
from langchain.prompts import PromptTemplate
import datetime

app = Flask(__name__)

# Initialize Groq language model

llm = ChatGroq(
    model="llama-3.1-8b-instant",
    temperature=0,
    api_key="___" #use your api key
)

# Arabic prompt template
template = """
أنت وكيل ذكي يتحكم في جهاز مروحة.
إذا كانت درجة الحرارة أكبر من 30 → القرار: "تشغيل المروحة".
إذا كانت درجة الحرارة أقل أو تساوي 30 → القرار: "إيقاف المروحة".

درجة الحرارة الحالية: {temperature}°م
ما القرار؟
اكتبه باللغة العربية فقط مع التفسير لماذا اتخذت هذا القرار.
"""


prompt = PromptTemplate(
    input_variables=["temperature"],
    template=template
)

@app.route('/fan', methods=['GET'])
def control_fan():
    # Get temperature value from query parameter
    temperature = request.args.get('temperature')

    # Validate input
    if not temperature:
        return jsonify({
            "status": "❌ ERROR",
            "message": "يرجى تمرير درجة الحرارة عبر معامل 'temperature'"
        }), 400

    try:
        temperature = float(temperature)
    except ValueError:
        return jsonify({
            "status": "❌ ERROR",
            "message": "صيغة درجة الحرارة غير صحيحة. يرجى إدخال رقم فقط."
        }), 400

    # Prepare the final prompt
    final_prompt = prompt.format(temperature=temperature)

    try:
        # Get response from Groq LLM
        response = llm.invoke(final_prompt)
        decision = response.content.strip()
    except Exception as e:
        return jsonify({
            "status": "❌ ERROR",
            "message": f"حدث خطأ أثناء الاتصال بـ Groq: {str(e)}"
        }), 500

    # Add visual emoji for decision
    if "تشغيل" in decision:
        emoji = "🟢"
    elif "إيقاف" in decision:
        emoji = "🔴"
    else:
        emoji = "⚙️"

    # Format output beautifully
    result = {
        "status": "✅ SUCCESS",
        "timestamp": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "data": {
            "temperature": f"{temperature}°م",
            "decision": f"{emoji} {decision}"
        },
        "note": "تم اتخاذ القرار بناءً على درجة الحرارة المدخلة."
    }

    return jsonify(result), 200


if __name__ == '__main__':
    # Run Flask app
    app.run(host='0.0.0.0', port=5000, debug=True)
