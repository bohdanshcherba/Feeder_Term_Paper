import mysql from 'mysql'

const db = mysql.createConnection({
    host:'localhost',
    user:'root',
    password:'pass',
    database:'cormushka'
})


const insert = (time, weight) =>{
    let data = {time_eating: time, weight_eating: weight}
    let sql = `INSERT INTO cormushka.data SET ?`
    let query = db.query(sql, data,(err,result)=>{

    })

}


export {db, insert}
