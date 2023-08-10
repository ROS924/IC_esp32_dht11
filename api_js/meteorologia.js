const db = require('./db')

const  Meteorologia = db.sequelize.define('meteorologia', {
    temperatura: {
        type: db.Sequelize.FLOAT
    },
    umidade: {
        type: db.Sequelize.FLOAT
    },
    indiceCalor: {
        type: db.Sequelize.FLOAT
    }
})

//Meteorologia.sync({force: true})

module.exports= Meteorologia