import {read as ardRead, write as ardWrite} from '../service/COMPorts.js'
import {db} from "../service/db.js";

const read = async (req,res) =>{
    res.send(ardRead())
}

const write = async (req,res) =>{
    console.log(req.body.payload)
    ardWrite(req.body.payload)
}

const getData =async (req,res) => {
    let sql = `SELECT * FROM cormushka.data ORDER BY id DESC`
    let query = await db.query(sql,(err,result)=>{
        res.send(result)
    })

}

export {
    read, write, getData
}
