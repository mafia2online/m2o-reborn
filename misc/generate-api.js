const fs = require('fs')

const exportPrefix = 'm2o_export'

const targets = [
    'server/api/errors.h',
    'server/api/vehicle.h',
];

(function () {
    /* parse the stuff */
    const results = targets
        .map(file => fs.readFileSync(file))
        .map(data => data.toString('utf8'))
        .map(data => data
            .split('\n')
            .filter(line => line.indexOf(exportPrefix) !== -1)
            .map(line => line
                .replace('{', '')
                .replace(exportPrefix, '')
                .trim()
            )
        )

    /* generate the internal headers */
    const internalHeader = results
        .map(lines => lines
            .map(line =>  line + ';')
            .join('\n')
        )
        .join('\n\n')

    fs.writeFileSync('server/api/api.h', internalHeader)

    /* generate public API headers */
    const matches = results
        .reduce((carry, elem) => carry.concat(elem), [])
        .map(line => line.match(/m2o_([_a-zA-Z0-9]+)/))

    const typedefs = matches
        .map(match => match.input.replace(match[0], `(api_${match[1]})`))
        .map(method => `typedef ${method};`)

    const vtable = matches
        .map(match => `api_${match[1]} *${match[1]};`)
        .map(line => ' '.repeat(4) + line)

    const publicHeaders = [
        typedefs.join('\n'),
        '',
        'typedef struct m2o_plugin_vtable {',
        vtable.join('\n'),
        '} m2o_plugin_vtable;',
    ];

    fs.writeFileSync('binary/includes/api.h', publicHeaders.join('\n'))
    fs.writeFileSync('server/api/public.h', publicHeaders.join('\n'))

    /* generate api vtable implementation */
    const apiImplementation = [
        'm2o_plugin_vtable m2o_api_init() {',
        '    m2o_plugin_vtable api;',
        '',
        matches
            .map(match => `api.${match[1]} = ${match[0]};`)
            .map(line => ' '.repeat(4) + line)
            .join('\n'),
        '',
        '    return api;',
        '}',
    ];

    fs.writeFileSync('server/api/api_impl.h', apiImplementation.join('\n'))
})()
